#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>


class Runway { // взлетная полоса
private:
	int id;
	bool occupied; // использует ли ее какой-то самолет
public:
	Runway(int _id) : id(_id), occupied(false)  {}

	friend std::ostream& operator<<(std::ostream& os, Runway *runway) {
		return os << runway->get_id() << " runway";
	}

	void occupy() { // в будущем можно добавить локи для многопоточности
		occupied = true;
		std::cout << this << " was occupied" << std::endl;
	}
	void release() {
		occupied = false;
		std::cout << this << " was released" << std::endl;
	}
	bool is_occupied() {
		return occupied;
	}
	int get_id() {
		return id;
	}
};


class Airport { // аэропорт
private:
	std::vector<Runway*> runways;
	std::string name;
public:
	Airport(std::string _name) : runways(std::vector<Runway*>()), name(_name) {}

	friend std::ostream& operator<<(std::ostream& os, Airport* airport) {
		return os << airport->get_name() << " " << " airport";
	}

	std::string get_name() {
		return name;
	}
	void build_runway(int id) {
		// если взлетной полосы с таким id еще не было, добавляем
		std::vector<Runway*>::iterator it = find_if(runways.begin(), runways.end(), [id](Runway * runway) { return runway->get_id() == id; });
		if (it == runways.end()) {
			Runway* runway = new Runway(id);
			std::cout << "new " << runway << " was added to " << this << std::endl;
			runways.push_back(runway);
		}
	}

	Runway* ask_for_free_runway() { // запрос на свободную полосу, ее может не быть, поэтому может вернуть nullptr.
		std::vector<Runway*>::iterator it = find_if(runways.begin(), runways.end(), [](Runway * runway) { return !runway->is_occupied();  });
		std::cout << this << " was asked for free runway, ";
		if (it == runways.end()) {
			std::cout << "no runway is free" << std::endl;
			return nullptr;
		}
		(*it)->occupy();
		std::cout << (*it) << " was given" << std::endl;
		return *it;
	}

	void release_runway(Runway* runway) { // возвращают свободную полосу
		std::vector<Runway*>::iterator it = find(runways.begin(), runways.end(), runway);
		std::cout << runway << " was released in " << this << std::endl;
		if (it == runways.end()) {
			std::cout << "there is no " << runway << " in " << this << std::endl;
			return;
		}
		(*it)->release();
	}

	~Airport() {
		for (Runway* runway : runways) {
			delete runway;
		}
	}

};

class Aircraft { // самолет
public:
	enum AircraftState { FLYING, LANDING, PARKED, TAKING_OFF };
private:

	std::string name;
	int id;
	Airport* airport; // текущий аэропорт, если самолет тут стоит или nullptr, если улетел
	Runway* runway; // текущая полоса, если взлетает/улетает
	AircraftState state;
	
public:
	// самолет изначально припаркован в каком-то аэропорту
	Aircraft(std::string _name, int _id, Airport *airport) : name(_name), id(_id), airport(airport), runway(nullptr), state(PARKED) {}

	friend std::ostream& operator<<(std::ostream& os, Aircraft* aircraft) {
		return os << aircraft->get_name() << " " << aircraft->get_id() << " aircraft";
	}

	std::string get_name() {
		return name;
	}
	int get_id() {
		return id;
	}
	AircraftState get_state() {
		return state;
	}
	void try_land_at(Airport* airport) { // пытаемся приземлиться в аэропорт
		if (state != FLYING) {
			std::cout << this << " is not flying, can't land" << std::endl;
			return;
		}
		std::cout << this << " tries to land in " << airport <<  std::endl;
		Runway* runway = airport->ask_for_free_runway();
		if (runway == nullptr) {
			std::cout << this << " is canceled for landing in " << airport << std::endl;
			return;
		}
		std::cout << this << " has landed in " << airport << " on " << runway << std::endl;
		this->airport = airport;
		this->runway = runway;
		state = LANDING;
	}

	void park() { // паркуемся после пасадки
		if (state != LANDING) {
			std::cout << this << "is not landing, can't park" << std::endl;
			return;
		}
		airport->release_runway(runway);
		runway = nullptr;
		state = PARKED;
	}

	void try_to_take_off() { // пытаемся взлететь
		if (state != PARKED) {
			std::cout << this << " is not parked, can't take off" << std::endl;
			return;
		}
		std::cout << this << " tries to take off from " << airport << std::endl;
		Runway* runway = airport->ask_for_free_runway();
		if (runway == nullptr) {
			std::cout << this << " is canceled for take off from " << airport << std::endl;
			return;
		}
		this->runway = runway;
		std::cout <<this << " has started to take off from " << airport << " from " << runway << std::endl;
		state = TAKING_OFF;
	}

	void fly() { // улетаем
		if (state != TAKING_OFF) {
			std::cout << this << " is not taking off, can't fly" << std::endl;
			return;
		}
		std::cout << this << " has taken off from " << airport << std::endl;
		airport->release_runway(runway);
		airport = nullptr;
		runway = nullptr;
		state = FLYING;
	}

};



int main() {
	Airport svo("Sheremetyevo");
	Airport dme("Domodedovo");

	svo.build_runway(1);
	svo.build_runway(2);
	svo.build_runway(3);

	dme.build_runway(1);
	dme.build_runway(2);

	Aircraft b737("boeing", 737, &svo);
	Aircraft b777("boeing", 777, &svo);
	Aircraft tu154("tu", 154, &svo);
	Aircraft a320("airbus", 320, &dme);
	Aircraft s100("superjet", 100, &dme);

	// проверяем взлет в шереметьево
	assert(b737.get_state() == Aircraft::AircraftState::PARKED);
	b737.try_to_take_off();
	assert(b737.get_state() == Aircraft::AircraftState::TAKING_OFF);
	b737.fly();
	assert(b737.get_state() == Aircraft::AircraftState::FLYING);

	// один взлетает в Домодедово, другой приземляется 
	a320.try_to_take_off();
	assert(a320.get_state() == Aircraft::AircraftState::TAKING_OFF);
	b737.try_land_at(&dme);
	assert(b737.get_state() == Aircraft::AircraftState::LANDING);
	// теперь в Домодедово все полосы заняты, s100 не сможет взлететь
	s100.try_to_take_off();
	assert(s100.get_state() == Aircraft::AircraftState::PARKED);
	a320.fly();
	// теперь a320 улетел, s100 может взлететь
	s100.try_to_take_off();
	assert(s100.get_state() == Aircraft::AircraftState::TAKING_OFF);
	// b737 приземляется, s100 улетает
	b737.park();
	assert(b737.get_state() == Aircraft::AircraftState::PARKED);
	s100.fly();
	assert(s100.get_state() == Aircraft::AircraftState::FLYING);
	// два самолета занмиают полосы для взлета в Шереметьево
	b777.try_to_take_off();
	assert(b777.get_state() == Aircraft::AircraftState::TAKING_OFF);
	tu154.try_to_take_off();
	assert(tu154.get_state() == Aircraft::AircraftState::TAKING_OFF);
	// один самолет занимает полосу для посадки
	a320.try_land_at(&svo);
	assert(a320.get_state() == Aircraft::AircraftState::LANDING);
	// Теперь второй самолет не получит полосу для посадки
	s100.try_land_at(&svo);
	assert(s100.get_state() == Aircraft::AircraftState::FLYING);
	b777.fly();
	assert(b777.get_state() == Aircraft::AircraftState::FLYING);
	// теперь s100 может приземлиться
	s100.try_land_at(&svo);
	assert(s100.get_state() == Aircraft::AircraftState::LANDING);
	s100.park();
	a320.park();
	tu154.fly();
	assert(tu154.get_state() == Aircraft::AircraftState::FLYING);
	assert(s100.get_state() == Aircraft::AircraftState::PARKED);
	assert(a320.get_state() == Aircraft::AircraftState::PARKED);

	return 0;
}