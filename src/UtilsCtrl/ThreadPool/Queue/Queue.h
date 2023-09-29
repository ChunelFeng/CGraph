/*******************************************************************
 * @file   Queue.h
 * @brief  引用计数实现的无锁队列
 *
 * @author PoLar_LinDa6
 * @date   2023.9.29
 *******************************************************************/

#include <atomic>
#include <new>
#include <unique_ptr>

template <typename T>
class Queue {
  struct node;
  struct counted_node_ptr {
    int external_count;
    node *ptr;
  };
  std::atomic<counted_node_ptr> head;
  std::atomic<counted_node_ptr> tail;

  struct node_counter {
    unsigned internal_count : 30;
    unsigned external_counters : 2;
  };
  struct node {
    std::atomic<T *> data;
    std::atomic<node_counter> count;
    std::atomic<counted_node_ptr> next;

    node() {
      node_counter new_count;
      new_count.internal_count = 0;
      new_count.external_counters = 2;
      count.store(new_count);

      counted_node_ptr new_next;
      new_next.ptr = nullptr;
      new_next.external_count = 0;
      next.store(new_next);
    }

    void release_ref() {
      node_counter old_counter = count.load(std::memory_order_relaxed);
      node_counter new_counter;
      do {
        new_counter = old_counter;
        --new_counter.internal_count;
      } while (!count.compare_exchange_strong(old_counter, new_counter,
                                              std::memory_order_acquire,
                                              std::memory_order_relaxed));

      if (!new_counter.internal_count && !new_counter.external_counters)
        delete this;
    }
  };

 private:
  static void increase_external_count(std::atomic<counted_node_ptr> &counter,
                                      counted_node_ptr &old_counter) {
    counted_node_ptr new_counter;
    do {
      new_counter = old_counter;
      ++new_counter.external_count;
    } while (!counter.compare_exchange_strong(old_counter, new_counter,
                                              std::memory_order_acquire,
                                              std::memory_order_relaxed));
    old_counter.external_count = new_counter.external_count;
  }

  static void free_external_counter(counted_node_ptr &old_node_ptr) {
    node *const ptr = old_node_ptr.ptr;
    int const count_increase = old_node_ptr.external_count - 2;
    node_counter old_counter = ptr->count.load(std::memory_order_relaxed);
    node_counter new_counter;
    do {
      new_counter = old_counter;
      --new_counter.external_counters;
      new_counter.internal_count += count_increase;
    } while (!ptr->count.compare_exchange_strong(old_counter, new_counter,
                                                 std::memory_order_acquire,
                                                 std::memory_order_relaxed));

    if (!new_counter.internal_count && !new_counter.external_counters)
      delete ptr;
  }

  void set_new_tail(counted_node_ptr &old_tail,
                    counted_node_ptr const &new_tail) {
    node *const current_tail_ptr = old_tail.ptr;

    while (!tail.compare_exchange_weak(old_tail, new_tail) &&
           old_tail.ptr == current_tail_ptr)
      ;
    if (old_tail.ptr == current_tail_ptr)
      free_external_counter(old_tail);
    else
      current_tail_ptr->release_ref();
  }

 public:
  Queue() : head(counted_node_ptr{1, new node}), tail(head.load()) {}
  Queue(Queue const &) = delete;
  Queue &operator=(Queue const &) = delete;
  ~Queue() {
    while (pop())
      ;
  }

  std::unique_ptr<T> pop() {
    counted_node_ptr old_head = head.load(std::memory_order_relaxed);
    for (;;) {
      increase_external_count(head, old_head);
      node *const ptr = old_head.ptr;
      if (ptr == tail.load().ptr) return {};

      counted_node_ptr next = ptr->next.load();
      if (head.compare_exchange_strong(old_head, next)) {
        T *const res = ptr->data.exchange(nullptr);
        free_external_counter(old_head);
        return std::unique_ptr<T>(res);
      }
      ptr->release_ref();
    }
  }

  void push(T new_value) {
    std::unique_ptr<T> new_data(new T(new_value));
    counted_node_ptr new_next;
    new_next.ptr = new node;
    new_next.external_count = 1;
    counted_node_ptr old_tail = tail.load();
    for (;;) {
      increase_external_count(tail, old_tail);
      T *old_data = nullptr;

      if (old_tail.ptr->data.compare_exchange_strong(old_data,
                                                     new_data.get())) {
        counted_node_ptr old_next = {0};

        if (!old_tail.ptr->next.compare_exchange_strong(old_next, new_next)) {
          delete new_next.ptr;
          new_next = old_next;
        }
        set_new_tail(old_tail, new_next);
        new_data.release();
        break;
      } else {
        counted_node_ptr old_next = {0};

        if (old_tail.ptr->next.compare_exchange_strong(old_next, new_next)) {
          old_next = new_next;
          new_next.ptr = new node;
        }
        set_new_tail(old_tail, old_next);
      }
    }
  }
};
