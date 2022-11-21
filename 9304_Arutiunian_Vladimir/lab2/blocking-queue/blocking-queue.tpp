template<typename T>
BlockingQueue<T>::BlockingQueue(std::string name, size_t max_size): name(std::move(name)), max_size(max_size) {};

template<typename T>
void BlockingQueue<T>::push(const T& value) {
    std::unique_lock lock(mutex);
    is_full_condition.wait(lock, [this]() { return this->queue.size() < max_size; });

    queue.push(value);

    print_text_with_thread_id("Element was added. Queue's size is " + std::to_string(queue.size()));
    is_empty_condition.notify_one();
}

template<typename T>
T BlockingQueue<T>::pop() {
    std::unique_lock lock(mutex);
    is_empty_condition.wait(lock, [this]() { return !this->queue.empty(); });

    auto front = queue.front();
    queue.pop();

    print_text_with_thread_id("Element was extracted. Queue's size is " + std::to_string(queue.size()));
    is_full_condition.notify_one();
    return front;
}

template<typename T>
void BlockingQueue<T>::print_text_with_thread_id(const std::string& text) const {
    std::stringstream temporary_stream;
    temporary_stream << "Thread id = [" << std::this_thread::get_id() << "]. Queue name = [" << name << "]. "
                     << text << '\n';
    std::cout << temporary_stream.str();
}