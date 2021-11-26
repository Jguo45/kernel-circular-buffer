Project 3
=========

Buffer
* test

Producer
* The producer is a function that uses a for loop to enqueue data into the buffer 100,000 times. Each loop starts by randomly sleeping for 0-10ms. The value `i` is used to determine which value to enqueue next by calculating `i % 10` and then using `memset` to repeat it 1024 times in an array. This array is then passed to `enqueue`. Finally, this function is passed to a `pthread` to run it concurrently with the consumer.

Consumer
* Similar to the producer, the consumer sleeps for between 0-10ms. It then passes a char array of size 1024 to `dequeue`. This consumer function is then also passed to a `pthread` to be run concurrently with the producer.

Test File
* How to compile kernel test file: `gcc test.c -lpthread -lrt -o test`
* How to compile userspace test file: `gcc test_user.c user_buffer.c -lpthread -lrt -o test`
