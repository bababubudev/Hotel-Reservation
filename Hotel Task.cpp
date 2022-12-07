#include <iostream>
#include <iomanip>
#include <string>

/* Header files containing all essential functions used here. */
#include "Constants.h"
#include "Utility.h"

/* contains important values to keep track when a reservation is done. */
struct reservation
{
	std::string r_name = std::string();
	
	bool is_set = false;
	bool is_booked = false;
	bool is_double = false;

	int r_id = -1;
	int nights = 0;
	
	double current_cost = 0.0;
};

/* Asks for input between 1-3 and returns that input */
int fetch_type()
{
	std::cout << "\nSelect room type!\n";
	std::cout << "1. [ Single room ]\n";
	std::cout << "2. [ Double room ]\n";
	std::cout << "3. [ Go to start ]\n\n";

	return validated_input<int>(1, 3);
}

/* 
Loops through all the possible values from our struct array, (here in a pointer form to not copy values around)
and returns the available room index as an int. boolean to check whether single rooms or double rooms are required.
*/
int fetch_room(const reservation *reserves, const bool want_doubles)
{
	constexpr int index = 0;

	/* Loops stops executing and returns once it has found an index corresponding to the required room. */
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

/* 
Calculates discounts randomly using discount_amount(), a header file function and finalizes the price.
Prints out the amount of discount added if required, controlled by a boolean parameter.
*/
void fetch_prices(reservation &room, const bool log)
{
	const int cost = room.is_double ? PRICE_D * room.nights : PRICE_S * room.nights;
	const int discount = discount_amount();

	if (log)
	{
		std::cout << "Accumulated cost: [" << cost << " EUR]\n";
		if (discount > 0)
			std::cout << "Price discount: [-" << static_cast<double>(cost) * discount * 0.01 << " EUR. (" << discount << "%)]\n\n";
		else
			std::cout << "Price discount: [No discounts!]\n\n";
	}

	room.current_cost = cost - (static_cast<double>(cost) * discount * 0.01);
}

#pragma region Logs

/* Logs choices given to the user in the beginning. */
void log_choices()
{
	std::cout << "What's on your mind?\n";
	std::cout << "1. [ Reserve a room ]\n";
	std::cout << "2. [ Your reservation ]\n";
	std::cout << "3. [ Cancel reservation ]\n";
	std::cout << "4. [ Quit application ]\n\n";
}

/* Logs the currently reserved room details (name, id, type, amount), boolean foreign is used to restrict some unneeded logging from  happening */
void log_reserved(reservation &current_room, const bool foreign = false)
{
	/* Defines the type as a string to later console out */
	const char* type = current_room.is_double ? "Double" : "Single";

	if (!foreign) std::cout << "\nYour room has been booked " << current_room.r_name << "\n";

	std::cout << "--------------------------------\n";
	std::cout << "Name: [" << current_room.r_name << "]\n";
	std::cout << "Room type: [" << type << "]\n";
	std::cout << "Nights: [" << current_room.nights << "]\n";
	std::cout << "ID: [" << current_room.r_id << "]\n\n";

	/* Boolean foreign is negated to only execute when on the first run */
	fetch_prices(current_room, !foreign);
	std::cout << "Total cost: [" << current_room.current_cost << " EUR]\n";
	std::cout << "--------------------------------\n\n";
}

/* A hidden function to use to debug booked rooms. */
void log_all(const reservation *rooms)
{
	std::cout << "\n";
	bool found = false;

	for (int i = 0; i < MAX_ROOMS; i++)
	{
		if (!rooms[i].is_booked) continue;

		std::string teller = rooms[i].r_name.empty() ? "Unknown" : rooms[i].r_name;

		if (rooms[i].is_double)
		{
			std::cout << i << ". [ Double ] Booked by: " << teller << ". [" << rooms[i].r_id << "]\n";
			found = true;
		}
		else
		{
			std::cout << i << ". [ Single ] Booked by: " << teller << ". [" << rooms[i].r_id << "]\n";
			found = true;
		}
	}

	const char* teller = found ? "\n" : "[ No booked rooms found! ]\n\n";
	std::cout << teller;
}

#pragma endregion

/* Function books room by populating the struct passed in via parameter */
void book_room(reservation &room, int &room_am)
{
	/* in case program fails or the rooms are booked fully */
	if (room.is_booked || room_am == 0)
	{
		std::cout << "\n[ Current room type is unavailable! ]\n\n";
		return;
	}

	std::cout << "[ Specify the number of nights to stay ]\n\n";
	validated_input(room.nights, 1, 365);

	room.is_booked = true;
	room.is_set = true;
	room.r_id = reserve_id();

	std::cout << "\n[ Specify the reservee's name ]\n\n";
	validated_input(room.r_name);

	log_reserved(room);

	/* lowers the amount of rooms */
	room_am--;
}

/* Depopulates by setting default values to all members in struct */
void remove_room(reservation &room)
{
	std::cout << "\nAre you sure you want to cancel this reservation " << room.r_name << "?\n";
	std::cout << "1. [ Yes! Cancel reservation! ]\n";
	std::cout << "2. [ No! Take me back! ]\n\n";

	const int choice = validated_input<int>(1, 2);

	switch (choice)
	{
	case 1:
		std::cout << "\n[ Reservation for " << room.r_name << " with ID " << room.r_id << " is now canceled! ]\n\n";
		room.is_booked = false;
		room.r_name = std::string();
		room.r_id = -1;
		room.nights = 0;
		room.current_cost = 0.0;
		break;
	case 2:
		std::cout << "\n";
		return;
	default: 
		break;
	}
}

/* Searches for canceled room and readds singles and doubles values that were previously removed. */
void release_room(const reservation *reserves, int &singles, int &doubles)
{
	const int ind_d = fetch_room(reserves, true);
	const int ind_s = fetch_room(reserves, false);

	/* if room is unbooked and had previously been set */
	if (!reserves[ind_d].is_booked && reserves[ind_d].is_set) doubles++; /* adds one more to double room amount */
	if (!reserves[ind_s].is_booked && reserves[ind_s].is_set) singles++; /* adds one more to single room amount */
}

/* Used to search, modify and redirect to other function relating to a singular reservation */
void manage_reservation(reservation *reserves, const bool cancellation = false)
{
	/* stores all the index found by searching through the list of reservations */
	int found_index[MAX_ROOMS];

	int id = 0;
	int amount = 0;
	bool is_str;

	const char* teller = cancellation ? "cancel" : "check";

	std::cout << "\n[ To " << teller << ", enter Room ID or Reservee's name ]\n\n";
	const std::string r_info = validated_input(true);

	/* try catch block to catch if the input is a string else setting id as the input */
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
		/* skip a loop if the room had previously been booked */
		if (!reserves[i].is_booked) continue;

		if (is_str)
		{
			/* if the stored name is equal to the input name */
			if (r_info == reserves[i].r_name)
			{
				found_index[amount] = i;
				amount++;
			}
		}
		else
		{
			/* if the stored id is equal to the input id */
			if (id == reserves[i].r_id)
			{
				found_index[amount] = i;
				amount++;
			}
		}
	}

	/* nothing is found by searching through */
	if (amount == 0)
	{
		std::cout << "\n[ " << r_info << " ] not found in the system!\n\n";
		return;
	}
	
	/* for the instances when we get the same number through rand() function */
	if (!is_str && amount > 1)
	{
		/* adds one in the id */
		reservation& reservee = reserves[found_index[0]];
		reservee.r_id = id + 1;

		std::cout << "\n[ Something went wrong! Reservee " << reservee.r_name << "'s id was renewed to " << reservee.r_id << " ]\n\n";

		/* returns out to be checked again*/
		return;
	}

	/* log out all the reserved ones */
	std::cout << "\nRoom booked by [" << r_info << "]\n";
	for (int i = 0; i < amount; i++)
	{
		log_reserved(reserves[found_index[i]], true);
	}

	/* if the function is used to cancel a booking */
	if (cancellation)
	{
		int choice = 0;

		/* if found results is more than one */
		if (amount > 1)
		{
			std::cout << "\n[ " << amount << " rooms found in your reservations! Please enter the corresponding number for the list above! ]\n\n";
			validated_input<int>(choice, 1, amount);
			
			/* because the index starts from 0 and not one */
			choice -= 1;
		}

		remove_room(reserves[found_index[choice]]);
	}
}

/* Mainly used to redirect to other function and controlling the what function to call at a given instruction. */
void controller(reservation *reserves, int &single_am, int &double_am, bool &wants_to_quit)
{
	/* Prints amount of available rooms at the beginning. */
	printf("AVAILABLE ROOMS\nsingle: %i, double: %i\n\n", single_am, double_am);
	log_choices();

	/* current_choice integer is asked to be inputted and switched later to specify which function to redirect to */
	const int current_choice = validated_input<int>(1, 5);

	switch (current_choice)
	{
	case 1:
		/* Breaks out prematurely incase all the rooms are empty so the program doesn't have to go through fetch_type() funciton */
		if (single_am + double_am == 0)
		{
			std::cout << "\n[ Apologies! All rooms are currently booked! ]\n\n";
			break;
		}

		/* similarly by using current_choice, fetch_type() that returns an int is used to specify the programs path. */
		switch (fetch_type())
		{
		case 1:
			std::cout << "\n";

			/* 
			book_room() takes a singluar reservation struct which is put in by using fetch_room() as an index of the reserves array
			*/
			book_room(reserves[fetch_room(reserves, false)], single_am);
			break;
		case 2:
			std::cout << "\n";

			/* 
			same here but boolean parameter is false in fetch_room() to search for double rooms
			double_am is amount of double_rooms
			*/
			book_room(reserves[fetch_room(reserves, true)], double_am);
			break;
		case 3:
			/* a return is used to return out of the function as 3rd option is to quit to main menu */
			return;
		default: 
			break;
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
		/* sets the value to be true which then stops while loop in main function*/
		std::cout << "\nAre you sure you want to quit?\n";
		std::cout << "1. [ Yes! Get me out! ]\n";
		std::cout << "2. [ No! It was a mistake! ]\n\n";
		
		if (validated_input<int>(1, 2) > 1)
		{
			std::cout << "\n";
			break;
		}
		else
		{
			wants_to_quit = true;
			std::cout << "\n";
			return;
		}

		break;
	case 5:
		log_all(reserves);
		break;
	default: 
		break;
	}
		
}

int main()
{
	/* boolean to stop program */
	bool wants_to_quit = false;

	/* header file function used to randomly select a room between 40 and 80 */
	const int r_division = room_division() * 2;

	/* which then gets divided into two */
	int single_room = r_division / 2, double_room = r_division / 2;

	/* sets an array to store all reservation data */
	reservation user_reservation[MAX_ROOMS];

	/* loop used to set up double rooms incase we need to access in some function */
	for (int i = 0; i < double_room; i++)
	{
		user_reservation[single_room + i].is_double = true;
	}

	/* main looper funciton, loops until user wants to quit */
	do
	{
		controller(user_reservation, single_room, double_room, wants_to_quit);
	}
	while (!wants_to_quit);

	return 0;
}