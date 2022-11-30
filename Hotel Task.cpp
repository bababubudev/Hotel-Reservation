#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <ctime>

const int MAX_ROOMS = 5;
const int DIVISIONS = 1;
const int MIN_ROOMS = 1;

struct Reservation
{
	std::string reserve_name = std::string();
	
	bool is_booked = false;
	bool is_double = false;

	int reservation_id = 0;
	int nights = 0;
};

#pragma region Util Functions

int reserve_id()
{
	srand((int)time(NULL));
	return rand() % 90000 + 10000;
}

int discount_amount()
{
	srand((int)time(NULL));
	return rand() % 3 * 10;
}

int room_division()
{
	srand((int)time(NULL));
	return rand() % (MAX_ROOMS / DIVISIONS) + MIN_ROOMS;
}

int fetch_room(Reservation * reserves, bool want_doubles)
{
	int index = 0;

	for (size_t i = 0; i < MAX_ROOMS; i++)
	{
		if (want_doubles)
		{
			if (reserves[i].is_double && !reserves[i].is_booked)
			{
				return i;
			}
		}
		else
		{
			if (!reserves[i].is_double && !reserves[i].is_booked)
			{
				return i;
			}
		}
	}

	return index;
}



#pragma endregion

#pragma region Input Validation

template<typename T>
void validated_input(T& input, int min = 0, int max = 0)
{
	std::cin >> std::setw(1) >> input;
	bool comparable = !(min == max);
	bool in_between = input >= min && input <= max;

	while (!std::cin.good() || (!in_between && comparable))
	{
		if (std::cin.good())
		{
			std::cout << "[ Only numbers from " << min << "-" << max << " are valid! ]\n";

			std::cin >> std::setw(1) >> input;
			in_between = input >= min && input <= max;
		}
		else
		{
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');

			std::cout << "[ Only numbers are allowed! ]\n";

			std::cin >> std::setw(1) >> input;
			in_between = input >= min && input <= max;
		}
	}

	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');
}

template<typename T>
T validated_input(int min = 0, int max = 0)
{
	T input;
	validated_input<T>(input, min, max);

	return input;
}

void validated_input(std::string& input, bool num_allowed = false)
{
	int checker = 0;
	bool success = false;

	std::getline(std::cin, input);

	if (num_allowed) return;

	while (!success)
	{
		try
		{
			checker = std::stoi(input);
			std::cout << "Name cannot start with a number: ";
			std::getline(std::cin, input);
		} catch (const std::exception&)
		{
			success = true;
		}
	}
}

std::string validated_input(bool num_allowed = false)
{
	std::string input;
	validated_input(input, num_allowed);

	return input;
}

#pragma endregion

#pragma region Logs

void show_choices()
{
	std::cout << "What's on your mind?\n";
	std::cout << "1. [ Reserve a room ]\n";
	std::cout << "2. [ Your reservations ]\n";
	std::cout << "3. [ Quit application ]\n\n";
}

void show_reserved(Reservation &current_room, bool ndef = false)
{
	const char* type = current_room.is_double ? "Double" : "Single";
	
	if (!ndef)
	{
		std::cout << "\n[ Your room has been booked " << current_room.reserve_name << " ]\n";
	}

	std::cout << "--------------------------\n";
	std::cout << "Name: [" << current_room.reserve_name << "]\n";
	std::cout << "Room type: [" << type << "]\n";
	std::cout << "Nights: [" << current_room.nights << "]\n";
	std::cout << "ID: [" << current_room.reservation_id << "]\n";
	std::cout << "--------------------------\n\n";
}

#pragma endregion

int room_type()
{
	std::cout << "\nSelect room type!\n";
	std::cout << "1. [ Single room ]\n";
	std::cout << "2. [ Double room ]\n\n";
	
	return validated_input<int>(1, 2);
}

void book_room(Reservation &room)
{
	if (room.is_booked)
	{
		std::cout << "\n[Room already booked!]\n\n";
		return;
	}

	std::cout << "Number of nights: ";
	validated_input(room.nights, 1, 365);

	room.is_booked = true;
	room.reservation_id = reserve_id();

	std::cout << "Enter your name: ";
	validated_input(room.reserve_name);

	show_reserved(room);
}

void check_reservation(Reservation* reserves)
{
	short int found_index[MAX_ROOMS];

	int id = 0;
	short int indexer = 0;
	bool is_str = false;

	std::cout << "\nEnter [Name] or [Room ID]: ";
	std::string r_info = validated_input(true);

	try
	{
		id = std::stoi(r_info);
		is_str = false;
	} catch (const std::exception&)
	{
		is_str = true;
	}


	for (size_t i = 0; i < MAX_ROOMS; i++)
	{
		if (!reserves[i].is_booked) continue;

		if (is_str)
		{
			if (r_info == reserves[i].reserve_name)
			{
				found_index[indexer] = i;
				indexer++;
			}
		}
		else
		{
			if (id == reserves[i].reservation_id)
			{
				found_index[indexer] = i;
				indexer++;
				return;
			}
		}
	}

	if (indexer == 0)
	{
		std::cout << "\n[ " << r_info << " ] not found in the system!\n\n";
		return;
	}
	else
	{
		std::cout << "Room booked by [" << r_info << "]\n";
		for (int i = 0; i < indexer; i++)
		{
			show_reserved(reserves[found_index[i]], true);
		}
	}
	
}

void controller(Reservation * reserves, int &single_am, int &double_am, bool &wants_to_quit)
{
	printf("AVAILABLE ROOMS\nsingle: %i, double: %i\n\n", single_am, double_am);
	show_choices();

	int current_choice = validated_input<int>(1, 3);

	switch (current_choice)
	{
	case 1:
		if (room_type() == 1)
		{
			int index = fetch_room(reserves, false);

			std::cout << "\n";
			book_room(reserves[index]);
			single_am--;
		}
		else
		{
			int index = fetch_room(reserves, true);

			std::cout << "\n";
			book_room(reserves[index]);
			double_am--;
		}

		break;
	case 2:
		check_reservation(reserves);
		break;
	case 3:
		wants_to_quit = true;
		break;
	}
}

int main()
{
	bool wants_to_quit = false;
	int booked_amount = 0;
	
	int r_division = room_division() * 2;
	int single_room = r_division / 2, double_room = r_division / 2;

	Reservation user_reservation[MAX_ROOMS];
	for (size_t i = 0; i < r_division; i++)
	{
		if (i > single_room)
		{
			user_reservation[i].is_double = true;
		}
	}

	user_reservation[0].is_booked = true;
	booked_amount++;
	
	do
	{
		controller(user_reservation, single_room, double_room, wants_to_quit);
	}
	while (booked_amount < (single_room + double_room) && !wants_to_quit);

	return 0;
}