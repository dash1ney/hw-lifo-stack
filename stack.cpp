#include "stack.hpp"
#include <map>
#include <cstddef>
#include <cstdlib>
#include <cstring>

namespace stack
{
    struct Element
    {
        void* data;
        Element* next;

        Element(const void* const data, const std::size_t size, Element* next)
        {
            this->data = malloc(size);
            memcpy(this->data, data, size);
            this->next = next;
        }

        ~Element()
        {
            free(data);
        }
    };

    struct Stack
    {
        Element* top;
        std::size_t size;

        Stack()
        {
            this->top = nullptr;
            this->size = 0;
        }

        ~Stack()
        {
            while (this->size > 0)
            {
                Element* toDelete = this->top;
                this->top = toDelete->next;
                delete toDelete;
                this->size--;
            }
        }
    };

    int stackCount = 0;
    std::map<Handle, Stack*> handleMap;

    Handle create()
    {
        Stack* stack = new Stack();
        handleMap[stackCount] = stack;
        return stackCount++;
    }

    void destroy(const Handle handle)
    {
        if (valid(handle))
        {
            delete handleMap[handle];
        }
    }

    bool valid(const Handle handle)
    {
        return handleMap.find(handle) != handleMap.end();
    }

    std::size_t count(const Handle handle)
    {
        return handleMap[handle]->size;
    }

    void push(const Handle handle, const void* const data, const std::size_t size)
    {
        Stack* stack = handleMap[handle];
        Element* element = new Element(data, size, stack->top);
        stack->top = element;
        stack->size++;
    }

    std::size_t pop(const Handle handle, void* const data, const std::size_t size)
    {
        Stack* stack = handleMap[handle];
        memcpy(data, stack->top->data, size);
        stack->top = stack->top->next;
        stack->size--;
        return size;
    }
}
