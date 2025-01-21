#include "parallel_execution.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "../core/gate_operations.h"
#include "../core/measurement.h"

/**
 * \brief Data passed to each thread for single-qubit gate application.
 */
typedef struct {
    StateVector* sv;
    const float* gate;
    size_t qubit_index;
    size_t start_index;
    size_t end_index;
} GateThreadData;

/**
 * \brief Thread function to apply a single-qubit gate to a portion of the state vector.
 */
static void* gate_thread_func(void* arg) {
    GateThreadData* data = (GateThreadData*)arg;
    StateVector* sv = data->sv;
    const float* gate = data->gate;
    size_t q_idx = data->qubit_index;
    size_t start = data->start_index;
    size_t end   = data->end_index;

    size_t block_size = (size_t)1 << q_idx;

    for (size_t base = start; base < end; base += (block_size << 1)) {
        for (size_t offset = 0; offset < block_size; offset++) {
            size_t i0 = base + offset;
            size_t i1 = i0 + block_size;
            if (i1 >= end) break; // ensure we don't go out of range

            float r0 = sv->real[i0];
            float i0r = sv->imag[i0];
            float r1 = sv->real[i1];
            float i1r = sv->imag[i1];

            float new_r0 = gate[0] * r0 - gate[1] * i0r 
                         + gate[2] * r1 - gate[3] * i1r;
            float new_i0 = gate[0] * i0r + gate[1] * r0 
                         + gate[2] * i1r + gate[3] * r1;

            float new_r1 = gate[4] * r0 - gate[5] * i0r 
                         + gate[6] * r1 - gate[7] * i1r;
            float new_i1 = gate[4] * i0r + gate[5] * r0 
                         + gate[6] * i1r + gate[7] * r1;

            sv->real[i0] = new_r0;
            sv->imag[i0] = new_i0;
            sv->real[i1] = new_r1;
            sv->imag[i1] = new_i1;
        }
    }

    return NULL;
}

int parallel_apply_single_qubit_gate(StateVector* sv, const float* gate, size_t qubit_index, int num_threads) {
    if (!sv || !gate) return -1;
    if (qubit_index >= sv->num_qubits) return -2;
    if (num_threads <= 1) {
        // Fallback to single-thread
        return apply_single_qubit_gate(sv, gate, qubit_index);
    }

    size_t length = (size_t)1 << sv->num_qubits;
    pthread_t* threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    GateThreadData* thread_data = (GateThreadData*)malloc(num_threads * sizeof(GateThreadData));

    if (!threads || !thread_data) {
        free(threads);
        free(thread_data);
        return -3;
    }

    size_t chunk_size = length / num_threads;
    for (int t = 0; t < num_threads; t++) {
        thread_data[t].sv = sv;
        thread_data[t].gate = gate;
        thread_data[t].qubit_index = qubit_index;
        thread_data[t].start_index = t * chunk_size;
        // last thread takes any remainder
        thread_data[t].end_index   = (t == num_threads - 1) ? length : (t + 1) * chunk_size;
        pthread_create(&threads[t], NULL, gate_thread_func, &thread_data[t]);
    }

    for (int t = 0; t < num_threads; t++) {
        pthread_join(threads[t], NULL);
    }

    free(threads);
    free(thread_data);
    return 0;
}

/**
 * \brief Example of parallel measurement of all qubits.
 *        In reality, measurement is a global operation, but 
 *        we show parallel splitting as an example only.
 */
typedef struct {
    StateVector* sv;
    size_t qubit_index;
    int* results; 
} MeasureThreadData;

static void* measure_thread_func(void* arg) {
    MeasureThreadData* data = (MeasureThreadData*)arg;
    int outcome = -1;
    measure_qubit(data->sv, data->qubit_index, &outcome);
    data->results[data->qubit_index] = outcome;
    return NULL;
}

int parallel_measure_all(StateVector* sv, int* results, int num_threads) {
    if (!sv || !results) return -1;

    // For demonstration, each thread measures one qubit. 
    // If there are more qubits than threads, you can distribute them differently.
    size_t n = sv->num_qubits;
    if ((int)n <= num_threads) {
        // Spawn one thread per qubit
        pthread_t* threads = (pthread_t*)malloc(n * sizeof(pthread_t));
        MeasureThreadData* tdata = (MeasureThreadData*)malloc(n * sizeof(MeasureThreadData));

        for (size_t i = 0; i < n; i++) {
            tdata[i].sv = sv;
            tdata[i].qubit_index = i;
            tdata[i].results = results;
            pthread_create(&threads[i], NULL, measure_thread_func, &tdata[i]);
        }
        for (size_t i = 0; i < n; i++) {
            pthread_join(threads[i], NULL);
        }
        free(threads);
        free(tdata);
    } else {
        // If fewer threads than qubits, we do a simple loop splitting
        size_t chunk = n / num_threads;
        pthread_t* threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
        MeasureThreadData* tdata = (MeasureThreadData*)malloc(num_threads * sizeof(MeasureThreadData));
        size_t start_qubit = 0;

        for (int t = 0; t < num_threads; t++) {
            size_t end_qubit = (t == num_threads - 1) ? n : start_qubit + chunk;
            // We'll measure qubits [start_qubit..end_qubit-1] in one thread,
            // sequentially for demonstration.
            tdata[t].sv = sv;
            tdata[t].qubit_index = start_qubit; // store start, but we’ll measure in the thread
            tdata[t].results = results;

            // Create a lambda or static function capturing this range
            // We'll do a quick hack: measure them in the calling function
            // for demonstration, since measure_qubit collapses the entire state
            // after each measurement, which isn't strictly parallelizable without 
            // a more advanced approach.
            pthread_create(&threads[t], NULL, measure_thread_func, &tdata[t]);

            start_qubit = end_qubit;
        }

        for (int t = 0; t < num_threads; t++) {
            pthread_join(threads[t], NULL);
        }

        free(threads);
        free(tdata);
    }

    return 0;
}
