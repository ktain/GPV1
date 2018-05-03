#if !defined(MENU_H)
#define MENU_H

extern bool busy;
extern int32_t menuEncCount;
extern int32_t ticksPerOption;

void refreshOptions(void);
void enter(void);

extern char* options[];

#endif
