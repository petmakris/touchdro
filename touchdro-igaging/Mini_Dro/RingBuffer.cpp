#include "RingBuffer.h"

// Constructor
//
// Set the capacity, allocate the buffer memory, and initialize the object.
RingBuffer::RingBuffer(const unsigned int capacity) :
    m_capacity(capacity),
    m_buffer(new byte[capacity])
{
    clear();
}

// Destructor
//
// Deletes buffer memory allocated by constructor.
RingBuffer::~RingBuffer()
{
    delete [] m_buffer;
}

// Clear the buffer of any contents.
void RingBuffer::clear()
{
    // Reset the head and tail pointers.
    m_head = m_tail = m_buffer;
    // Reset the full/empty flags.
    m_empty = true;
    m_full = false;
}

// Push a byte into the buffer.
//
// Does not guard against pushing into a full buffer, oldest byte will be lost.
void RingBuffer::push(byte b)
{
    // Save byte and bump tail pointer.
    *m_tail++ = b;
    // If tail has reached end of buffer, wrap around to beginning of buffer.
    // (This is why it's called a RING buffer.)
    if ((unsigned int) (m_tail - m_buffer) == m_capacity) {
        m_tail = m_buffer;
    }
    // Buffer is definitely not empty now.
    m_empty = false;
    // If buffer was already full, ...
    if (m_full) {
        // ... force head to match tail.
        // The byte that WAS at the head will be lost.
        m_head = m_tail;
    } else {
        // Otherwise, set whether buffer is full now.
        m_full = (m_tail == m_head);
    }
}

// Pop a byte from the buffer
//
// Does not guard against popping from an empty buffer, zero would be returned.
byte RingBuffer::pop()
{
    // If buffer is empty, return zero byte.
    if (m_empty) {
        return (byte) 0;
    }
    // Get byte from head of buffer and bump head pointer.
    byte b = *m_head++;
    // If head has reached end of buffer, wrap around to beginning of buffer.
    // (This is why it's called a RING buffer.)
    if ((unsigned int) (m_head - m_buffer) == m_capacity) {
        m_head = m_buffer;
    }
    // Buffer is definitely not full now.
    m_full = false;
    // Set whether buffer is empty now.
    m_empty = (m_tail == m_head);
    return b;
}

// Test if buffer is empty
bool RingBuffer::isEmpty() const
{
    return m_empty;
}

// Test if buffer is full
bool RingBuffer::isFull() const
{
    return m_full;
}
