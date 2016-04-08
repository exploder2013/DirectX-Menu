#include "main.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

rManager		rMGR	= rManager();
D3Menu			d3		= D3Menu();

int					screenX = GetSystemMetrics(SM_CXSCREEN);
int					screenY = GetSystemMetrics(SM_CYSCREEN);
const				MARGINS margins = { -1, -1, -1, -1 };


VOID renderOverlay() {
	rMGR.clear();

	d3.render();

	rMGR.present();
}

INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{

	WNDCLASSEX		cx;
	HWND			hWnd = NULL;

	cx.cbClsExtra = NULL;
	cx.cbSize = sizeof(WNDCLASSEX);
	cx.cbWndExtra = NULL;
	cx.hbrBackground = (HBRUSH)0;
	cx.hCursor = LoadCursor(NULL, IDC_ARROW);
	cx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	cx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	cx.hInstance = hInstance;
	cx.lpfnWndProc = WndProc;
	cx.lpszClassName = L"Cross";
	cx.lpszMenuName = NULL;
	cx.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&cx))
		MessageBox(NULL, L"Couldn't register class", NULL, NULL), exit(0);

	hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW, L"Cross", L"Cross", WS_POPUP, 0, 0, screenX, screenY, NULL, NULL, hInstance, NULL);
	SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 255, ULW_COLORKEY | LWA_ALPHA);

	ShowWindow(hWnd, nShowCmd);

	if (!rMGR.Initilize(hWnd, screenX, screenY))
		MessageBox(NULL, L"Couldn't initialize device", NULL, NULL), exit(0);

	if (!d3.Initilize(&rMGR))
		MessageBox(NULL, L"Couldn't initialize D3Menu", NULL, NULL), exit(0);

	d3.createMenu( "menu1" );
	d3.createMenu( "menu2" );

	INT v1 = 10;
	FLOAT v2 = 20;
	string v3 = "oh hai johnny!";

	d3.createItem( "item1", &v1, D3DCOLOR_ARGB(255,0,0,140), 10, VAR_TYPE::T_INT, nullptr );
	d3.createItem( "item2", &v2, D3DCOLOR_ARGB(255,0,0,140), 1.5, VAR_TYPE::T_FLOAT, nullptr );
	d3.createItem( "item3", &v3, D3DCOLOR_ARGB(255,0,0,140), 10, VAR_TYPE::T_STRING, nullptr );


	MSG msg;
	while (TRUE)
	{
		d3.handleInput();
		renderOverlay();

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	rMGR.~rManager();

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_PAINT:
			renderOverlay();
			break;
		case WM_CREATE:
			DwmExtendFrameIntoClientArea(hWnd, &margins);
			break;
		case WM_DESTROY:

			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}