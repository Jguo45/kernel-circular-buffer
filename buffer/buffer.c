#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include "buffer.h"

static ring_buffer_421_t buffer;
static struct semaphore mutex;
static struct semaphore fill_count;
static struct semaphore empty_count;

SYSCALL_DEFINE0(init_buffer_421) {
	node_421_t *node;
	node_421_t *curr;
	int i;

	// Note: You will need to initialize semaphores in this function.
	// Ensure we're not initializing a buffer that already exists.
	if (buffer.read || buffer.write) {
		printk("init_buffer_421(): Buffer already exists. Aborting.\n");
		return -1;
	}

	// Create the root node.
	node = (node_421_t *) kmalloc(sizeof(node_421_t), GFP_KERNEL);
	// Create the rest of the nodes, linking them all together.

	curr = node;
	// Note that we've already created one node, so i = 1.
	for (i = 1; i < SIZE_OF_BUFFER; i++) {
		curr->next = (node_421_t *) kmalloc(sizeof(node_421_t), GFP_KERNEL);
		curr = curr->next;
	}
	// Complete the chain.
	curr->next = node;
	buffer.read = node;
	buffer.write = node;
	buffer.length = 0;

	// Initialize your semaphores here.
	// TODO
	sema_init(&mutex, 1);
	sema_init(&fill_count, 0);
	sema_init(&empty_count, SIZE_OF_BUFFER);

	return 0;
}

SYSCALL_DEFINE1(enqueue_buffer_421, char*, data) {
	long err_count;
	// decrements empty_count
	down(&empty_count);
	
	// NOTE: You have to modify this function to use semaphores.
	if (!buffer.write) {
		printk("write_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}

	if (buffer.length >= SIZE_OF_BUFFER) {
		return -1;
	}

	// closes mutex
	down(&mutex);

	// memcpy(buffer.write->data, data, DATA_LENGTH);
	// if (copy_from_user(buffer.write->data, data, DATA_LENGTH) != 0) {
	// 	printk("Failed to read data: %c\n", buffer.write->data[0]);
	// }
	err_count = copy_from_user(buffer.write->data, data, DATA_LENGTH);
	if (err_count != 0) {
		printk("Enqueue: Failed to copy %ld bytes\n", err_count);
	}

	// Advance the pointer.
	buffer.write = buffer.write->next;
	buffer.length++;

	printk("Enqueued: %c\n", data[0]);
	
	// opens mutex
	up(&mutex);

	// increments fill_count
	up(&fill_count);

	return 0;
}

SYSCALL_DEFINE1(dequeue_buffer_421, char*, data) {
	long err_count;
	// decrements fill_count
	down(&fill_count);

	// NOTE: Implement this function.
	if (!buffer.write) {
		printk("dequeue_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}
	if (buffer.length <= 0) {
		return -1;
	}

	// closes mutex
	down(&mutex);

	err_count = copy_to_user(data, buffer.read->data, DATA_LENGTH);
	if (err_count != 0) {
		printk("Dequeue: Failed to copy %ld bytes\n", err_count);
	}

	buffer.read = buffer.read->next;
	buffer.length--;

	printk("Dequeued: %c\n", data[0]);
	
	// opens mutex
	up(&mutex);

	// increments empty_count
	up(&empty_count);

	return 0;
}

SYSCALL_DEFINE0(delete_buffer_421) {
	node_421_t *temp;
	node_421_t *curr;

	// Tip: Don't call this while any process is waiting to enqueue or dequeue.
	if (!buffer.read) {
		printk("delete_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}
	// Get rid of all existing nodes.
	curr = buffer.read->next;

	while (curr != buffer.read) {
		temp = curr->next;
		kfree(curr);
		curr = temp;
	}
	// Free the final node.
	kfree(curr);
	curr = NULL;

	// Reset the buffer.
	buffer.read = NULL;
	buffer.write = NULL;
	buffer.length = 0;
	
	return 0;
}
