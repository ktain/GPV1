#if !defined(MENU_H)
#define MENU_H

extern bool busy;
extern int32_t menuEncCount;

void refreshOptions(void);
void enter(void);

extern char* options[];

#endif
