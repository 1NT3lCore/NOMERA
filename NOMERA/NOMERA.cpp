#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <stdexcept>

class Room {
private:
    int roomNumber;
    int floor;
    int totalBeds;
    int occupiedBeds;
    bool isBooked;
    std::string residentGender;

public:
    Room(int roomNumber, int floor, int totalBeds)
        : roomNumber(roomNumber), floor(floor), totalBeds(totalBeds), occupiedBeds(0), isBooked(false), residentGender("") {}

    int getRoomNumber() const { return roomNumber; }
    int getFloor() const { return floor; }
    int getTotalBeds() const { return totalBeds; }
    int getOccupiedBeds() const { return occupiedBeds; }
    bool getIsBooked() const { return isBooked; }
    std::string getResidentGender() const { return residentGender; }

    void bookRoom(bool isBooked, std::string residentGender) {
        this->isBooked = isBooked;
        this->residentGender = residentGender;
    }

    void setOccupiedBeds(int occupiedBeds) {
        if (occupiedBeds <= totalBeds && occupiedBeds >= 0) {
            this->occupiedBeds = occupiedBeds;
        }
        else {
            throw std::invalid_argument("Неверное количество занятых спальных мест");
        }
    }

    int getTotalOccupiedBeds() const {
        return isBooked ? occupiedBeds : 0;
    }
};

class RoomCollection {
private:
    std::vector<Room> rooms;

public:
    void addRoom(int roomNumber, int floor, int totalBeds) {
        rooms.push_back(Room(roomNumber, floor, totalBeds));
    }

    int getTotalOccupiedBeds() const {
        int total = 0;
        for (const auto& room : rooms) {
            total += room.getTotalOccupiedBeds();
        }
        return total;
    }

    void getBookedRooms() const {
        std::cout << "Список номеров с бронью:\n";
        for (const auto& room : rooms) {
            if (room.getIsBooked()) {
                std::cout << "Номер: " << room.getRoomNumber()
                    << ", Этаж: " << room.getFloor()
                    << ", Спальных мест: " << room.getTotalBeds()
                    << ", Занятых спальных мест: " << room.getOccupiedBeds()
                    << ", Поселен: " << room.getResidentGender() << "\n";
            }
        }
    }

    void bookRandomRooms(int numRoomsToBook) {
        srand(static_cast<unsigned>(time(0)));

        for (int i = 0; i < numRoomsToBook; ++i) {
            int roomIndex = rand() % rooms.size();
            Room& room = rooms[roomIndex];

            if (!room.getIsBooked()) {
                bool isBooked = true;
                std::string residentGender = (rand() % 2 == 0) ? "мужчина" : "женщина";
                room.bookRoom(isBooked, residentGender);

                // Генерируем количество занятых мест от 1 до 3
                int occupiedBeds = rand() % (room.getTotalBeds()) + 1; // Убедитесь, что кол-во мест не превышает общее количество

                try {
                    room.setOccupiedBeds(occupiedBeds);
                }
                catch (const std::invalid_argument& e) {
                    std::cerr << "Ошибка при установке занятых мест в комнате " << room.getRoomNumber() << ": " << e.what() << "\n";
                    continue; // Пропускаем эту комнату и переходим к следующей
                }

                // Добавляем жильцов в комнату
                for (int j = 1; j < occupiedBeds; ++j) { // Начинаем с 1, так как уже добавили первого жильца
                    residentGender = (rand() % 2 == 0) ? "мужчина" : "женщина";
                    room.bookRoom(isBooked, residentGender);
                }
            }
        }
    }
};

int main() {
    setlocale(LC_ALL, "RU");
    RoomCollection collection;

    // Добавляем несколько номеров в коллекцию
    collection.addRoom(101, 2, 2);
    collection.addRoom(102, 3, 3);
    collection.addRoom(103, 2, 2);
    collection.addRoom(104, 1, 1);

    // Бронируем несколько номеров случайным образом
    collection.bookRandomRooms(3);

    // Вычисляем общее количество занятых спальных мест
    int totalOccupiedBeds = collection.getTotalOccupiedBeds();
    std::cout << "Общее количество занятых спальных мест: " << totalOccupiedBeds << "\n";

    // Выводим список номеров с бронью
    collection.getBookedRooms();

    return 0;
}
