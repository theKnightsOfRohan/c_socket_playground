#include "input_handler.h"
#include "utils.h"
#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void *handle_input(void *arg) {
	assert(arg != NULL);

	input_buffer *input = (input_buffer *)arg;

	// Notification lock should be locked by the main thread
	assert(pthread_mutex_trylock(&input->lock) == EBUSY);

	while (true) {
		printf("Enter a message: ");
		fgets(input->buffer, sizeof(input->buffer) - 1, stdin);

		// Remove newline character
		input->buffer[strcspn(input->buffer, "\n")] = '\0';

		if (strncmp(input->buffer, "quit", 4) == 0) {
			printf("Client is quitting\n");
		}

		// Notify the main thread that input is ready
		assert(pthread_mutex_unlock(&input->lock) == 0);

		printf("Input handler is waiting for the main thread to process the input\n");

		printf("Input thread awaiting lock confirmation\n");
		assert(pthread_mutex_lock(&input->lock) == 0);

		input->buffer[0] = '\0';
		// Do the weird clear thing
		// fgets(input->buffer, sizeof(input->buffer) - 1, stdin);
		// input->buffer[0] = '\0';
	}
}