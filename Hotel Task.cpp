#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <ctime>

const int MAX_ROOMS = 80;
const int MAX_RESERVES = 2;

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
	return rand() % (MAX_ROOMS / 4) + 20;
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
			std::cout << "\n[ Only numbers between " << min << "-" << max << " are valid! ]\n\n";

			std::cin >> std::setw(1) >> input;
			in_between = input >= min && input <= max;
		}
		else
		{
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');

			std::cout << "\n[ Only numbers are allowed! ]\n\n";

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

void validated_input(std::string& input)
{
	std::cin >> std::setw(32) >> input;
	while (!std::cin.good())
	{
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
		std::cout << "Max characters (32) reached! Try again: ";
		std::cin >> std::setw(32) >> input;
	}

	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');
}


std::string validated_input()
{
	std::string input;
	validated_input(input);

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
		std::cout << "\n[ Room is booked! ]\n";
	}

	std::cout << "--------------------------\n";
	std::cout << "Name: " << current_room.reserve_name << "\nRoom type: " << type << "\nID: " << current_room.reservation_id << "\nNights: " << current_room.nights;
	std::cout << "\n--------------------------\n\n";
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
	int id = 0;
	bool is_str = false;

	std::cout << "\nEnter [Name] or [Room ID]: ";
	std::string r_info = validated_input();

	try
	{
		id = std::stoi(r_info);
		is_str = false;
	} catch (const std::exception&)
	{
		is_str = true;
	}


	int amount = 0;
	for (size_t i = 0; i < MAX_ROOMS; i++)
	{
		if (is_str)
		{
			if (r_info == reserves[i].reserve_name)
			{
				amount++;
				show_reserved(reserves[i], true);
			}
		}
		else
		{
			if (id == reserves[i].reservation_id)
			{
				show_reserved(reserves[i], true);
				return;
			}
		}
	}
}

void controller(Reservation * reserves, int &single_am, int &double_am)
{
	int current_choice = 0;

	printf("Single: %i, Double: %i\n", single_am, double_am);
	show_choices();
	validated_input(current_choice, 1, 3);

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
		return;
	}
}

int main()
{
	int booked_amount = 0;
	
	int division = room_division() * 2;
	int single_room = division / 2, double_room = division / 2;

	Reservation user_reservation[MAX_ROOMS];
	for (size_t i = 0; i < division; i++)
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
		controller(user_reservation, single_room, double_room);
	}
	while (booked_amount < (single_room + double_room));

	return 0;
}