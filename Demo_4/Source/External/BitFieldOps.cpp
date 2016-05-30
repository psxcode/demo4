#define PREFERENCE_LIKES_ICE_CREAM (1 << 0) // 0x01
#define PREFERENCE_PLAYS_GOLF      (1 << 1) // 0x02
#define PREFERENCE_WATCHES_TV      (1 << 2) // 0x04
#define PREFERENCE_READS_BOOKS     (1 << 3) // 0x08
 
unsigned char preference;
 
void set_preference(unsigned char flag) {
    preference |= flag;
}
 
void reset_preference(unsigned char flag) {
    preference &= ~flag;
}
 
unsigned char get_preference(unsigned char flag) {
    return (preference & flag) != 0;
}

unsigned char toggle_preference( unsigned char flag ) {
	return preference ^= flag;
}