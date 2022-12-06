#include <iostream>
#include <iomanip>
#include <string>

#include "Constants.h"
#include "Utility.h"

struct Reservation
{
	std::string reserve_name = std::string();
	
	bool is_booked = false;
	bool is_double = false;

	int reservation_id = -1;
	int nights = 0;
	
	double current_cost = 0.0;
};

int fetch_room(Reservation* reserves, bool want_doubles)
{
	int index = 0;

	for (int i = 0; i < MAX_ROOMS; i++)
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

void calculate_prices(Reservation& room, bool log)
{
	int cost = room.is_double ? PRICE_D * room.nights : PRICE_S * room.nights;
	int discount = discount_amount();

	if (log)
	{
		std::cout << "Accumulated cost: [" << cost << " EUR]\n";
		if (discount > 0)
			std::cout << "Price discount: [-" << (double)cost * discount * 0.01 << " EUR. (" << discount << "%)]\n\n";
		else
			std::cout << "Price discount: [No discounts!]\n\n";
	}

	room.current_cost = cost - ((double)cost * discount * 0.01);
}

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
			std::cout << "\n[ Only numbers from " << min << " : " << max << " are valid! ]\n\n";

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
			std::cout << "\n[ Name cannot start with a number ]\n\n";
			std::getline(std::cin, input);
		} 
		catch (const std::exception&)
		{
			if (input == "")
			{
				std::cout << "\n[ Please enter your name before you continue ]\n\n";
				std::cin.clear();
				std::getline(std::cin, input);
				continue;
			}

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

void log_choices()
{
	std::cout << "What's on your mind?\n";
	std::cout << "1. [ Reserve a room ]\n";
	std::cout << "2. [ Your reservations ]\n";
	std::cout << "3. [ Quit application ]\n\n";
}

void log_reserved(Reservation &current_room, bool foreign = false)
{
	const char* type = current_room.is_double ? "Double" : "Single";

	if (!foreign) std::cout << "\n[ Your room has been booked " << current_room.reserve_name << " ]\n";

	std::cout << "--------------------------\n";
	std::cout << "Name: [" << current_room.reserve_name << "]\n";
	std::cout << "Room type: [" << type << "]\n";
	std::cout << "Nights: [" << current_room.nights << "]\n";
	std::cout << "ID: [" << current_room.reservation_id << "]\n\n";

	calculate_prices(current_room, !foreign);
	std::cout << "Total cost: [" << current_room.current_cost << " EUR]\n";
	std::cout << "--------------------------\n\n";
}

void log_rooms(Reservation * rooms)
{
	std::cout << "\n";
	for (int i = 0; i < MAX_ROOMS; i++)
	{
		if (!rooms[i].is_booked) continue;

		std::string teller = rooms[i].reserve_name == "" ? "Unknown" : rooms[i].reserve_name;

		if (rooms[i].is_double)
		{
			std::cout << i << ". [ Double ] Booked by: " << teller << "\n";
			
		}
		else
		{
			std::cout << i << ". [ Single ] Booked by: " << teller << "\n";
		}
	}
	std::cout << "\n";
}

#pragma endregion

int room_type()
{
	std::cout << "\nSelect room type!\n";
	std::cout << "1. [ Single room ]\n";
	std::cout << "2. [ Double room ]\n";
	std::cout << "3. [ Go to start ]\n\n";
	
	return validated_input<int>(1, 3);
}

void book_room(Reservation& room)
{
	if (room.is_booked)
	{
		std::cout << "\n[ Room already booked! ]\n\n";
		return;
	}

	std::cout << "[ Specify the number of nights to stay ]\n\n";
	validated_input(room.nights, 1, 365);

	room.is_booked = true;
	room.reservation_id = reserve_id();

	std::cout << "\n[ Specify the reservor's name ]\n\n";
	validated_input(room.reserve_name);

	log_reserved(room);
}

void check_reservation(Reservation* reserves)
{
	short int found_index[MAX_ROOMS];

	int id = 0;
	short int indexer = 0;
	bool is_str = false;

	std::cout << "\n[ Search for reservee name or the room id ]\n\n";
	std::string r_info = validated_input(true);

	try
	{
		id = std::stoi(r_info);
		is_str = false;
	} 
	catch (const std::exception&)
	{
		is_str = true;
	}


	for (int i = 0; i < MAX_ROOMS; i++)
	{
		if (!reserves[i].is_booked || reserves[i].reservation_id == -1) continue;

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
			}
		}
	}

	if (indexer == 0)
	{
		std::cout << "\n[ " << r_info << " ] not found in the system!\n\n";
		return;
	}

	if (!is_str && indexer > 1)
	{
		Reservation reservee = reserves[found_index[0]];
		reservee.reservation_id = id + 1;
		std::cout << "\n[ Something went wrong! Reservee " << reservee.reserve_name <<"'s id was renewed to " << reservee.reservation_id << " ]\n\n";
		return;
	}

	std::cout << "\nRoom booked by [" << r_info << "]\n";
	for (int i = 0; i < indexer; i++)
	{
		log_reserved(reserves[found_index[i]], true);
	}
}

void controller(Reservation * reserves, int &single_am, int &double_am, bool &wants_to_quit)
{
	printf("AVAILABLE ROOMS\nsingle: %i, double: %i\n\n", single_am, double_am);
	log_choices();

	int current_choice = validated_input<int>(1, 4);

	switch (current_choice)
	{
	case 1:
		switch (room_type())
		{
		case 1:
			std::cout << "\n";

			book_room(reserves[fetch_room(reserves, false)]);
			single_am--;
			break;
		case 2:
			std::cout << "\n";

			book_room(reserves[fetch_room(reserves, true)]);
			double_am--;
			break;
		case 3:
			return;
		}
		break;
	case 2:
		check_reservation(reserves);
		break;
	case 3:
		wants_to_quit = true;
		break;
	case 4:
		log_rooms(reserves);
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
	for (int i = 0; i < double_room; i++)
	{
		user_reservation[single_room + i].is_double = true;
	}

	do
	{
		controller(user_reservation, single_room, double_room, wants_to_quit);
	}
	while (booked_amount < (single_room + double_room) && !wants_to_quit);

	return 0;
}