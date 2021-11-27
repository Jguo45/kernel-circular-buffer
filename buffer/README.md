Project 3
=========
Name: Johnny Guo and Ruben Philips
Date: 11/28/2021
Class: CMSC 421-03 and CMSC 421-02 respectively

Overview
  Create a circular buffer in Debian with four system calls that initialize the linked list, enqueue and dequeue an item, and delete the list to free memory. The function names used for the system calls are the following: init_buffer_421, enqueue_buffer_421, dequeue_buffer_421, and delete_buffer_421. init_buffer_421 and delete_buffer_421 implementations were given by professor/TAs.

Buffer
  init_buffer_421(void)
    Implementation was already given in the assignment, the only change made was the initializing of the semaphores.
   
  enqueue_buffer_421(char* data)
    Copies inputted data and checks if buffer is initialized and if there is available space. If buffer is uninitialized, data is not inserted and a -1 is returned to indicate a failed enqueue. If the buffer is initialized, semaphore empty_count is decremented and semaphore mutex is closed to block the caller if the buffer is full. If the buffer is full and there is no available space, data is not inserted and the caller is blocked. If the buffer is initialized and the list is not full, data is inserted and the list length is incremented. Semaphore mutex is opened, semaphore fill_count is incremented, and a 0 is returned to indicate a sucessful enqueue.
    
  dequeue_buffer_421(char* data)
    Dequeues the current data in the read node after checking if the buffer is initialized and is not empty. If the buffer is uninitialized, a -1 is returned to indicate a failed dequeue. Else, semaphore fill_count is decremented and semaphore mutex is closed to block the caller if the buffer is empty. If the buffer is empty, nothing is dequeued and the caller is blocked. If the buffer is initialized and is not empty, the data in the current read node is dequeued and read is updated to its next node. The list length is also decremented. Semaphore mutex is opened, semaphore empty_count is incremented, and a 0 is returned to indicate a sucessful dequeue.
    
  delete_buffer_421(void)
    Implementation was already given in the assignment.

Producer
  The producer is a function that uses a for loop to enqueue data into the buffer 100,000 times. Each loop starts by randomly sleeping for 0-10ms. The value `i` is used to determine which value to enqueue next by calculating `i % 10` and then using `memset` to repeat it 1024 times in an array. This array is then passed to `enqueue`. Finally, this function is passed to a `pthread` to run it concurrently with the consumer.

Consumer
  Similar to the producer, the consumer sleeps for between 0-10ms. It then passes a char array of size 1024 to `dequeue`. This consumer function is then also passed to a `pthread` to be run concurrently with the producer.

Test File
* How to compile kernel test file: `gcc test.c -lpthread -lrt -o test`
* How to compile userspace test file: `gcc test_user.c user_buffer.c -lpthread -lrt -o test`
