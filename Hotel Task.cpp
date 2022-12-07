#include <iostream>
#include <iomanip>
#include <string>

#include "Constants.h"
#include "Utility.h"

struct Reservation
{
	std::string reserve_name = std::string();
	
	bool is_set = false;
	bool is_booked = false;
	bool is_double = false;

	int reservation_id = -1;
	int nights = 0;
	
	double current_cost = 0.0;
};

int fetch_type()
{
	std::cout << "\nSelect room type!\n";
	std::cout << "1. [ Single room ]\n";
	std::cout << "2. [ Double room ]\n";
	std::cout << "3. [ Go to start ]\n\n";

	return validated_input<int>(1, 3);
}

int fetch_room(Reservation *reserves, bool want_doubles)
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

void fetch_prices(Reservation &room, bool log)
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

#pragma region Logs

void log_choices()
{
	std::cout << "What's on your mind?\n";
	std::cout << "1. [ Reserve a room ]\n";
	std::cout << "2. [ Your reservation ]\n";
	std::cout << "3. [ Cancel reservation ]\n";
	std::cout << "4. [ Quit application ]\n\n";
}

void log_reserved(Reservation &current_room, bool foreign = false)
{
	const char* type = current_room.is_double ? "Double" : "Single";

	if (!foreign) std::cout << "\nYour room has been booked " << current_room.reserve_name << "\n";

	std::cout << "--------------------------\n";
	std::cout << "Name: [" << current_room.reserve_name << "]\n";
	std::cout << "Room type: [" << type << "]\n";
	std::cout << "Nights: [" << current_room.nights << "]\n";
	std::cout << "ID: [" << current_room.reservation_id << "]\n\n";

	fetch_prices(current_room, !foreign);
	std::cout << "Total cost: [" << current_room.current_cost << " EUR]\n";
	std::cout << "--------------------------\n\n";
}

void log_all(Reservation *rooms)
{
	std::cout << "\n";
	bool found = false;

	for (int i = 0; i < MAX_ROOMS; i++)
	{
		if (!rooms[i].is_booked) continue;

		std::string teller = rooms[i].reserve_name == "" ? "Unknown" : rooms[i].reserve_name;

		if (rooms[i].is_double)
		{
			std::cout << i << ". [ Double ] Booked by: " << teller << ". [" << rooms[i].reservation_id << "]\n";
			found = true;
		}
		else
		{
			std::cout << i << ". [ Single ] Booked by: " << teller << ". [" << rooms[i].reservation_id << "]\n";
			found = true;
		}
	}

	const char* teller = found ? "\n" : "[ No booked rooms found! ]\n\n";
	std::cout << teller;
}

#pragma endregion

void book_room(Reservation &room)
{
	if (room.is_booked)
	{
		std::cout << "\n[ Room already booked! ]\n\n";
		return;
	}

	std::cout << "[ Specify the number of nights to stay ]\n\n";
	validated_input(room.nights, 1, 365);

	room.is_booked = true;
	room.is_set = true;
	room.reservation_id = reserve_id();

	std::cout << "\n[ Specify the reservor's name ]\n\n";
	validated_input(room.reserve_name);

	log_reserved(room);
}

void remove_room(Reservation &room)
{
	std::cout << "\nAre you sure you want to cancel this reservation " << room.reserve_name << "?\n";
	std::cout << "1. [ Yes! Cancel reservation! ]\n";
	std::cout << "2. [ No! Take me back! ]\n\n";

	int choice = validated_input<int>(1, 2);

	switch (choice)
	{
	case 1:
		std::cout << "\n[ Reservation for " << room.reserve_name << " with ID " << room.reservation_id << " is now cancelled! ]\n\n";
		room.is_booked = false;
		room.reserve_name = std::string();
		room.reservation_id = -1;
		room.nights = 0;
		room.current_cost = 0.0;
		break;
	case 2:
		std::cout << "\n";
		return;
	}
}

void release_room(Reservation *reserves, int &singles, int &doubles)
{
	int ind_d = fetch_room(reserves, true);
	int ind_s = fetch_room(reserves, false);

	if (!reserves[ind_d].is_booked && reserves[ind_d].is_set)
	{
		printf("\n%i released.\n\n", ind_d);
		doubles++;
	}

	if (!reserves[ind_s].is_booked && reserves[ind_s].is_set)
	{
		printf("\n%i released.\n\n", ind_s);
		singles++;
	}
}

void manage_reservation(Reservation *reserves, bool cancellation = false)
{
	int found_index[MAX_ROOMS];

	int id = 0;
	int amount = 0;
	bool is_str = false;

	const char* teller = cancellation ? "cancel" : "check";

	std::cout << "\n[ To " << teller << ", enter Room ID or Reservee's name ]\n\n";
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
		if (!reserves[i].is_booked) continue;

		if (is_str)
		{
			if (r_info == reserves[i].reserve_name)
			{
				found_index[amount] = i;
				amount++;
			}
		}
		else
		{
			if (id == reserves[i].reservation_id)
			{
				found_index[amount] = i;
				amount++;
			}
		}
	}

	if (amount == 0)
	{
		std::cout << "\n[ " << r_info << " ] not found in the system!\n\n";
		return;
	}
		
	if (!is_str && amount > 1)
	{
		Reservation& reservee = reserves[found_index[0]];
		reservee.reservation_id = id + 1;
		std::cout << "\n[ Something went wrong! Reservee " << reservee.reserve_name << "'s id was renewed to " << reservee.reservation_id << " ]\n\n";
		return;
	}

	//if (cancellation) return;

	std::cout << "\nRoom booked by [" << r_info << "]\n";
	for (int i = 0; i < amount; i++)
	{
		log_reserved(reserves[found_index[i]], true);
	}

	if (cancellation)
	{
		int choice = 0;
		if (amount > 1)
		{
			std::cout << "\n[ " << amount << " rooms found in your reservations! Please enter the corresponding number for the list above! ]\n\n";
			validated_input<int>(choice, 1, amount);
			choice -= 1;
		}

		remove_room(reserves[found_index[choice]]);
	}
}

void controller(Reservation *reserves, int &single_am, int &double_am, bool &wants_to_quit)
{
	printf("AVAILABLE ROOMS\nsingle: %i, double: %i\n\n", single_am, double_am);
	log_choices();

	int current_choice = validated_input<int>(1, 5);

	switch (current_choice)
	{
	case 1:
		switch (fetch_type())
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
		manage_reservation(reserves);
		break;
	case 3:
		manage_reservation(reserves, true);
		release_room(reserves, single_am, double_am);
		break;
	case 4:
		wants_to_quit = true;
		break;
	case 5:
		log_all(reserves);
		break;
	}
		
}

int main()
{
	bool wants_to_quit = false;
	
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
	while (!wants_to_quit);

	//TODO: Restrict from reserving unavailable rooms. Stop appropriately when rooms are full.
	//TODO: 

	return 0;
}