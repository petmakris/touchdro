#ifndef _RING_BUFFER__H_
#define _RING_BUFFER__H_

#include <Arduino.h>


 
class RingBuffer
{
    private:
        unsigned int m_capacity;    // maximum bytes buffer can hold
        byte*        m_buffer;      // pointer to buffer memory
        byte*        m_head;        // head pointer, where next pop will occur
        byte*        m_tail;        // tail pointer, where next push will occur
        bool         m_empty;       // true if buffer is empty
        bool         m_full;        // true if buffer is full
        
    public:
        /**
         * Constructor
         *
         * Allocates buffer memory and initializes object.
         *
         * @param capacity maximum number of bytes buffer will hold
         */
        RingBuffer(const unsigned int capacity);
        
        /**
         * Destructor
         */
        virtual ~RingBuffer();
        
        /**
         * Clear contents of buffer
         *
         * This method resets the head and tail pointers, and marks the buffer
         * as empty and not-full.
         */
        void clear();
        
        /**
         * Push a byte into the back of the buffer
         *
         * The pushed byte can only be popped out of the buffer after all bytes
         * already pushed in are popped out first.  If the buffer is already
         * full before the push, then the byte at the front of the buffer will
         * be lost.  If this is not desired, check whether the buffer is full
         * BEFORE performing the push.
         *
         * @param b byte to be pushed into the buffer
         */
        void push(byte b);
        
        /**
         * Pop a byte from the front of the buffer
         *
         * If the buffer is not empty, the byte pushed in earliest will be
         * popped out and returned.  If the buffer is empty, a zero byte will be
         * returned.  If this is not the desired behavior, check whether the
         * buffer is empty BEFORE performing the pop.
         *
         * @return byte popped from the buffer
         */
        byte pop();
        
        /**
         * Check if the buffer is empty
         *
         * @return true if empty, false otherwise
         */
        bool isEmpty() const;
        
        /**
         * Check if the buffer is full
         *
         * @return true if full, false otherwise
         */
        bool isFull() const;
};

#endif
