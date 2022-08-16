#if !defined(__MESSAGEW_H)
#define __MESSAGEW_H
/*
 * MessageW.h -
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 */
//						H - files
#include <owl\owlpch.h>
#include <bwcc.h>
/*
 * 	Function's prototypes
 */
void DrawBorder(HDC, RECT&, int); 	// Рисование рамочки
BOOL Delay(TWindow*, WORD);			// Delay any time
void MessageW(	const char* WindCaption,
					const char* MsgSTRING);
/*
 *  -------------------------------------------------------------------------
 * 	Class TtestsystWindow
 *  -------------------------------------------------------------------------
 */
class TMesgWindow : public TWindow {
  public:
	unsigned WaitTimeOut;			// Время вывешивания сообщения
	TMesgWindow(TWindow*, const char* far Title, TModule* module = 0);
											// Prototips
	void 	Paint					(TDC&, BOOL erase, TRect&);
	void  GetWindowClass		(WNDCLASS& wndClass);
	char far* GetClassName	();
};

#endif  // __MESSAGEW_H
/*
 * ---------------------------------------------------------
 *	EoF(MessageW.h)
 * ----------------------------------------------------------
 */
