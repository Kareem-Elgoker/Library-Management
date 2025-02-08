#ifndef OWNER_H
#define OWNER_H

	#include <stdbool.h>
	#include "types.h"
	
	bool owner_auth(u8 * username, u8 * password);
	
	u32 owner_choices(void);
	bool owner_flow(u32 choice);
#endif //OWNER_H