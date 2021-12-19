#include <iostream>
#include <cstdlib>

using namespace std;

namespace infinity {

    template <class T>
    class MStack {
    private:
        T* m_stack{};
        unsigned int m_size{};

    private:
        //在栈区后面追加append_size 个 T类型的字节
        bool alloc_append(unsigned int append_size) {

            if (empty()) {
                //空栈初始化大小
                //(void)calloc(unsigned int num,unsigned int size);
                auto init_allocSize = append_size * sizeof(T);
                m_stack = (T*)calloc(append_size, init_allocSize);
                if (m_stack) {
                    ++m_size;
                    return true;
                }

            }
            else {
                auto re_alloc = new T[m_size + append_size];
                if (re_alloc) {
                    memcpy(re_alloc, m_stack, alloc_size(m_size));
                    delete[] m_stack;
                    m_stack = re_alloc;
                    ++m_size;
                    return true;
                }
            }

            return false;
        }

        bool alloc_erase(unsigned int erase_size) {
            if (!empty()) {
                auto stack_change = realloc(m_stack, alloc_size(m_size - erase_size));
                if (stack_change || m_size == 1) {
                    m_size -= erase_size;
                    return true;
                }
            }
            return false;
        }

    private:
        auto stack_top_index() {
            return this->m_size - 1;
        }

        auto stack_bottom_index() {
            return 0;
        }

    protected:
        T* operator [] (const unsigned int index) const {
            if (m_size)
                if (index >= 0 && index < m_size)
                    return (m_stack + index);
            return nullptr;
        }

    public:
        MStack() = default;

        ~MStack() {
            clear();
        }

        bool push(const T& data) {
            if (alloc_append(1)) {
                *(stack_top()) = data;
                return true;
            }
            return false;
        }

        bool pop() {
            return alloc_erase(1);
        }

        bool clear() {
            return alloc_erase(m_size);
        }

        unsigned int size() const {
            return m_size;
        }

        inline bool empty() {
            return m_size == 0;
        }

        inline T* stack_bottom() {
            return (*this)[stack_bottom_index()];
        }

        inline T* stack_top() {
            return (*this)[stack_top_index()];
        }

        inline unsigned int alloc_size(unsigned int length) {
            return length * sizeof(T);
        }

        inline unsigned int alloc_size() {
            return sizeof(T);
        }
    };

    template <class T>
    class MStackPlus{
    public:
        std::vector<T>m_stack;

    public:
        MStackPlus() {

        }

        void push(const T& data) {
            m_stack.push_back(data);
        }

        bool pop() {
            if (!m_stack.size())
                return false;
            m_stack.pop_back();
            return true;
        }

        bool empty() {
            return m_stack.empty();
        }

        T stack_top() const{
            return m_stack[m_stack.size() - 1];
        }

        T stack_bottom() const {
            return m_stack[0];
        }

        T operator [] (const unsigned int& index) const{
            return m_stack[index];
        }
        
    };

}