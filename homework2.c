/*************************************
 * Homework 2 - Local Network Checker
 *
 * This program takes two IP addresses, and determines if they are on the same local 
 * network. If the addresses are longer than 12 digits and 3 periods (i.e. 123.456.789.100),
 * the excess is not read.
 *
 * I was shooting for stability, as my initial attempts weren't very robust with input.
 * This program should be able to accept erroneous input without complaint, but it will
 * not be accurate with it's results.
 *
 * Author: 	Garrett Bates
 * Email:	gbates1@kent.edu 
 * Repository:	https://cs43203.dstan.net/gbates1/gbatesrepo.git
 *
 *************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Summary: Takes two integers and a mask (also an integer), and checks if the relevant bits are equal.
// What you need: two integers and a mask.
// What you get: A char containing either 1 (equal) or 0 (not equal).
char bitcompare(int first_ip, int second_ip, int subnet_mask) {
	int c1, c2;			// Stores the results of the mask application.
	char result; 			// Used this in place of bool variable. Does the same thing.

	c1 = first_ip & subnet_mask;	// Compare the first IP to subnet mask.
	c2 = second_ip & subnet_mask;	// Compare the second IP to subnet mask.

	if (c1 == c2) result = 1;	// If both comparisons yield the same value, return true.
	else result = 0;		// Otherwise, return false.

	return result; 
}

// Summary: Reads an IP address in dotted format, and returns a 32-bit number representation.
// What you need: a pointer to a string that contains an IP address in dotted format (must be of type char *).
// What you get: a 32-bit number representation of the IP address.
unsigned int address_parser(char *input) {
	char *n, *copy, *temp;		// These are used to parse delimited input.
	unsigned int i=0,ip=0,numba[4];	// These are used to store input as numbers.
	unsigned int shift=32;

	copy = strdup(input);		// Since strtok_r destroys the input string, use a copy.

	for(i = 0; (n = strtok_r(copy, ".", &temp)) != NULL; i++) { // When strtok_r returns null, break loop.
		if (i == 4) break;
		copy = NULL; 		// This must be null for subsequent calls to strtok_r.
		numba[i] = atoi(n);	// This converts each delimited string to a number.
	}

	int j;
	for(j = 0; j < i; j++) {	// Iterates through the integers and shifts them appropriately
		shift = shift - 8;	// Shift numba[0] 32 left, shift numba[1] 24 left, etc.
		numba[j] = numba[j]<<shift; // Does the actual shifting
		ip = ip + numba[j];	// Adds each value together. Serves the same purpose as bitwise or.
	}

	return ip;			// Return the integer representation
}

int main(){
	unsigned int ip1, ip2, submask;	// Storage for addresses.
	char *address;
	char local;
	size_t *size = malloc(16);

	printf("Enter your subnet mask: ");
	fflush(stdin);			// Flush stdin of any lingering input.
	getline(&address, size, stdin);	// Reads input.
	submask = address_parser(address);

	printf("Enter your first IP address: ");
	fflush(stdin);
        getline(&address, size, stdin);
	ip1 = address_parser(address);

	printf("Enter your second IP address: ");
	fflush(stdin);
        getline(&address, size, stdin);
	ip2 = address_parser(address);

	local = bitcompare(ip1, ip2, submask);
	
	if (local) printf("These IP addresses are on the same network.\n");
	else printf("These IP addresses are NOT on the same network.\n");

	return 0;
}
	
