#ifndef PAYMENTS_H
#define PAYMENTS_H

	#include <stdbool.h>
	#include "types.h"
	
	// return -1 for invalid ID, -2 for out of stock, otherwise return the price.
	s32 payment(u32 book_id);
	void cash_confirm(void);
	bool card_payment(User * user, u64 total_money);
	
#endif //PAYMENTS_H