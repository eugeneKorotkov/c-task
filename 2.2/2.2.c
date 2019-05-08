#include <stdio.h>

#define ERROR -1
#define OK 0

struct Node {
	int id;
	struct Node* next;
};

struct LinkedList {
	int size;
	struct Node* head;
};

int push_front(struct LinkedList *list, int id) { // добавляет узел в начало списка
	struct Node* new_node = malloc(sizeof(struct Node)); // создаем новый узел
	if (new_node == NULL) {
		return ERROR;
	}
	new_node->id = id;
	new_node->next = list->head;
	list->head = new_node; // он становится головой списка
	list->size++;
	return OK;
}

int insert_on(struct LinkedList* list, int pos, int id) { // вставляет узел на позицию pos (считая с нуля)
	if (list->size < pos) {
		return ERROR;
	}
	if (list->size == 0) { 
		push_front(list, id);
		return OK;
	}
	struct Node* cur = list->head;
	for (int i = 0; i < pos - 1; i++) { // после цикла в cur лежит узел, после которого нужно вставить
		cur = cur->next;
	}
	struct Node* new_node = malloc(sizeof(struct Node)); // создаем новый узел
	if (new_node == NULL) {
		return ERROR;
	}
	new_node->id = id;
	new_node->next = cur->next;
	cur->next = new_node;
	list->size++;
	return OK;
}

int push_back(struct LinkedList* list, int id) { // добавляет узел в конец списка
	return insert_on(list, list->size, id);
}

int erase_front(struct LinkedList* list) { // удаляет первый элемент
	if (list->size == 0) {
		return ERROR;
	}
	struct Node* to_erase = list->head;
	list->head = list->head->next;
	free(to_erase);
	list->size--;
	return OK;
}

int erase_from(struct LinkedList* list, int pos) { // удаляет узел, стоящий на позиции pos
	if (pos >= list->size) {
		return ERROR;
	}
	if (pos == 0) {
		return erase_front(list);
	}
	struct Node* cur = list->head;
	for (int i = 0; i < pos - 1; i++) { // после цикла в cur лежит узел, после которого нужно удалить
		cur = cur->next;
	}
	struct Node* to_erase = cur->next;
	cur->next = cur->next->next;
	free(to_erase);
	list->size--;
	return OK;
}

int erase_back(struct LinkedList* list) { // удаляет узел из конца
	return erase_from(list, list->size - 1);
}

void print(struct LinkedList* list) { // выводит id всех узлов списка по порядку
	struct Node* cur = list->head;
	while (cur != NULL) {
		printf("%d ", cur->id);
		cur = cur->next;
	}
	printf("\n");
}

struct LinkedList* create_list() { // создает пустой лист
	struct LinkedList* list = malloc(sizeof(struct LinkedList));
	list->head = NULL;
	list->size = 0;
}

void erase(struct LinkedList* list) { // удаляет лист (освобождает память)
	while (list->size) {
		erase_front(list);
	}
	free(list);
}


int main() {
	struct LinkedList* list = create_list();
	push_back(list, 1);
	push_back(list, 2);
	push_front(list, 3); // после этого должно быть 3 1 2

	print(list);
	
	insert_on(list, 1, 10);
	insert_on(list, 1, 15); 
	insert_on(list, 5, 7); // после этого должно быть 3 15 10 1 2 7

	print(list);

	erase_back(list);
	erase_front(list);
	erase_from(list, 2);
	erase_from(list, 1); // после этого должно быть 15 2

	print(list);

	erase(list);
	return 0;
}



