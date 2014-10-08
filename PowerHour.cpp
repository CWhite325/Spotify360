#include <stdlib.h>
#include <string>
#include <iostream>
#include <Windows.h>
#include <Xinput.h>
#include "CXBOXController.h"
#include <iostream>
#include "Timer.h"

using namespace std;

CXBOXController* Player1;
CXBOXController* Player2;
CXBOXController* Player3;
CXBOXController* Player4;
HWND spotify;

UINT APP_CONSTANT = 0x0319;
int NONE        = 0;
int	PLAYPAUSE   = 917504;
int	MUTE        = 524288;
int	VOLUMEDOWN  = 589824;
int	VOLUMEUP    = 655360;
int	STOP        = 851968;
int	PREVIOUS    = 786432;
int	NEXT        = 720896;

bool P1round = false;
bool P2round = false;
bool P3round = false;
bool P4round = false;
bool P1veto = false;
bool P2veto = false;
bool P3veto = false;
bool P4veto = false;
int veto = 0;
int round = 0;
int P1drinks = 0;
int P2drinks = 0;
int P3drinks = 0;
int P4drinks = 0;
using namespace std; 

static HWND getSpotify() {
	return FindWindow("SpotifyMainWindow", NULL);
}

static void doSpotify(HWND spotify, int command) {

	SendMessage(spotify, APP_CONSTANT, 0, command);

}

static void roundReset() {

	P1round = false;
	P2round = false;
	P3round = false;
	P4round = false;

}

static string getInfo(HWND spotify) {
	LPSTR s = new char[255];
	GetWindowText(spotify, s, 255);
	string x = s;
	if (x.length() > 9) {
	x = x.substr(9);
	string np = x.substr(x.find_first_of("\x96")+2);
	np += " by" + x.substr(0, x.length()-np.length()-3);
	return "Now Playing: " + np;
	}
	return "Spotify is not playing!";
}

static void doDisplay(HWND spotify) {

	system("cls");
	cout << getInfo(spotify) << endl;
	cout << "Round " << round << ":" << endl;
	if(Player1->IsConnected()) {
		cout << "\nPlayer 1\nScore: " << P1drinks << "\n" << "Veto: " << (P1veto ?  "Yes" : "No") << endl;
	}
	if(Player2->IsConnected()) {
		cout << "\nPlayer 2\nScore: " << P2drinks << "\n" << "Veto: " << (P2veto ?  "Yes" : "No") << endl;
	}
	if(Player3->IsConnected()) {
		cout << "\nPlayer 3\nScore: " << P3drinks << "\n" << "Veto: " << (P3veto ?  "Yes" : "No") << endl;
	}
	if(Player4->IsConnected()) {
		cout << "\nPlayer 4\nScore: " << P4drinks << "\n" << "Veto: " << (P4veto ?  "Yes" : "No") << endl;
	}
}

static void vetoReset() {

	P1veto = false;
	P2veto = false;
	P3veto = false;
	P4veto = false;
	veto = 0;

}

static char* strrev2(char* s) {

	char* x = _strdup(s);
	int size = strlen(s);
	for (int i=1; i<=size; i++) {
		s[i-1] = x[size-i]; 
	}
	return s;
}

int main(int argc, char* argv[]) {
	char x[] = {'h', 'e', 'l', 'l', 'o', '\0'};
	strrev2(x);
	cout << x;
	spotify = getSpotify();
    Player1 = new CXBOXController(1);
	Player2 = new CXBOXController(2);
	Player3 = new CXBOXController(3);
	Player4 = new CXBOXController(4);
	time_t start;
    start = time(NULL);
	time_t cur;
	Sleep(1000);
	string curTrack = getInfo(spotify);
	doDisplay(spotify);

    while(true)
    {
		if(curTrack != getInfo(spotify)) {
			curTrack = getInfo(spotify);
			vetoReset();
			doDisplay(spotify);
		}
		cur = time(NULL);
		if((int)difftime(start,cur)%60 == 0) {
			round++;
			doDisplay(spotify);
			roundReset();
			//PlaySound(TEXT("one_minute_to_win.wav"), NULL, SND_ASYNC);
			Sleep(2000);
		}

		if (veto >= 1) {
			doSpotify(spotify, NEXT);
			doDisplay(spotify);
			vetoReset();
			Sleep(2000);
		}

        if(Player1->IsConnected()) {
            if(Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A) {
				if (P1round == false) {
					P1drinks++;
					P1round = true;
					doDisplay(spotify);
				}
            }
			if(Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X) {
				if(P1veto == false) {
					veto++;
					P1veto = true;
					doDisplay(spotify);
				}
			}
			if(Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) {
				doSpotify(spotify, VOLUMEUP);
			}
			if(Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
				doSpotify(spotify, VOLUMEDOWN);
			}
		}

		if(Player2->IsConnected()) {
            if(Player2->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A) {
				if (P2round == false) {
					P2drinks++;
					doDisplay(spotify);
					P2round = true;
				}
            }
			if(Player2->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X) {
				if(P2veto == false) {
					veto++;
					P2veto = true;
					doDisplay(spotify);
				}
			}
		}   

		if(Player3->IsConnected()) {
            if(Player3->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A) {
				if (P3round == false) {
					P3drinks++;
					P3round = true;
					doDisplay(spotify);
				}
            }
			if(Player3->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X) {
				if(P3veto == false) {
					veto++;
					doDisplay(spotify);
					P3veto = true;
				}
			}
		}

		if(Player4->IsConnected()) {
            if(Player4->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A) {
				if (P4round == false) {
					P4drinks++;
					P4round = true;
					doDisplay(spotify);
				}
            }
			if(Player4->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X) {
				if(P4veto == false) {
					veto++;
					doDisplay(spotify);
					P4veto = true;
				}
			}
		}
    }

    delete(Player1);
	delete(Player2);
	delete(Player3);
	delete(Player4);

    return( 0 );
}