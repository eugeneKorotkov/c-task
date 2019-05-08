#include <iostream>
#include <string>
#include <stdexcept>

template <class T>
class LinkedList {
private:

	class Node { // делаем класс вложенным, чтобы он был недоступен снаружи листа
	private:
		T info; // полезная информация, хранящаяся в узле
		Node* next;
	public:
		void set_next(Node* next) {
			this->next = next;
		}
		Node* get_next() {
			return next;
		}
		T get_info() {
			return info;
		}
		Node(T _info) : info(_info), next(nullptr) {}
	};

	Node* head;
	size_t size;

public:

	LinkedList() : head(nullptr), size(0) {}

	void print() {
		for (Node* cur = head; cur != nullptr; cur = cur->get_next()) {
			std::cout << cur->get_info() << " ";
		}
		std::cout << std::endl;
	}

	void push_front(T info) { // добавляет узел в начало списка
		Node* new_node = new Node(info);
		new_node->set_next(head);
		head = new_node;
		size++;
	}

	void insert_on(size_t pos, T info) { // вставляет узел на позицию pos (считая с нуля)
		if (size < pos) {
			throw std::out_of_range("");
		}
		if (size == 0) {
			push_front(info);
			return;
		}
		Node* cur = head;
		for (int i = 0; i < pos - 1; i++) { // после цикла в cur лежит узел, после которого нужно вставить
			cur = cur->get_next();
		}
		Node* new_node = new Node(info);
		new_node->set_next(cur->get_next());
		cur->set_next(new_node);
		size++;
	}

	void push_back(T info) { // добавляет узел в конец списка
		insert_on(size, info);
	}

	void erase_front() { // удаляет первый элемент
		if (size == 0) {
			return; // решил не кидать исключение, так как, например, std::vector их не кидает
		}
		Node* to_erase = head;
		head = head->get_next();
		delete to_erase;
		size--;
	}

	void erase_from(size_t pos) { // удаляет узел, стоящий на позиции pos
		if (pos >= size) {
			throw std::out_of_range("");
		}
		if (pos == 0) {
			erase_front();
		}
		Node* cur = head;
		for (int i = 0; i < pos - 1; i++) { // после цикла в cur лежит узел, после которого нужно удалить
			cur = cur->get_next();
		}
		Node* to_erase = cur->get_next();
		cur->set_next(cur->get_next()->get_next());
		delete to_erase;
		size--;
	}

	void erase_back() { // удаляет узел из конца
		return erase_from(size - 1);
	}

	void clear() {
		while (size) {
			erase_front();
		}
	}

	~LinkedList() {
		clear();
	}

};

int main() {

	LinkedList<std::string> list;
	list.push_front("world");
	list.push_front(",");
	list.push_front("hello");
	list.print(); // hello , world

	list.insert_on(1, "a");
	list.insert_on(1, "b");
	list.insert_on(5, "be"); // hello b a , world be
	list.print(); // hello b a , world be
	 
	list.erase_back();
	list.erase_front();
	list.erase_from(2);
	list.erase_from(1);
	list.print(); // b world

	list.clear();
	list.print(); // 

	try {
		list.insert_on(1, "a");
	}
	catch (const std::out_of_range& e) {
		std::cout << "exception was thrown, OK" << std::endl;
	}

	return 0;
}

