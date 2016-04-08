#include "D3Menu.h"

D3Menu::D3Menu()
{
	// EMPTY
}

bool D3Menu::Initilize(rManager * rMGR)
{
	if (rMGR)
	{
		this->rMGR = rMGR;
		return true;
	}

	return false;
}

VOID D3Menu::render()
{
	if( !isOpen )
		return VOID();

	SIZE_T posY			= 0;
	SIZE_T loopStep		= 0;

	if (currentEntry) // MENU IS OPEN, SO DRAW ONLY IT'S CONTAINERS
	{
		
		for (PMENUITEM item : currentEntry->items) // DRAW ITEMS
		{
			DWORD color = item->color;

			if (item->isEnabled && loopStep == step ) {
				color = D3DCOLOR_ARGB( fade, 0, 255, 0 );
			} else if (item->isEnabled) {
				color = D3DCOLOR_ARGB( 255, 0, 0, 255 );
			}
				

			string text = item->itemName;

			if (item->value != nullptr) {
				switch (item->type)
				{
					case VAR_TYPE::T_INT:
					{
						text += "<" + to_string(*(INT*)item->value) + ">";
						break;
					}
					case VAR_TYPE::T_FLOAT:
					{
						text += "<" + to_string(*(FLOAT*)item->value) + ">";
						break;
					}
					case VAR_TYPE::T_STRING:
					{
						text += "<" + *(string*)item->value + ">";
						break;
					}
					default:
					{
						continue;
					}
				}
			} else {
				text += "<>";
			}
			

			rMGR->drawText( item->itemName.c_str(), 0, posY, color );
			
			posY += 14;
			loopStep++;
		}

	} else { // NO MENU IS OPEN, DRAW EVERYTHING

		for (PMENUENTRY entry : menus) // DRAW MENUS
		{
			DWORD color = entry->color;
			if (loopStep == step) {
				color = D3DCOLOR_ARGB(fade, 0, 0, 255);
			}

			rMGR->drawText( entry->entryName.c_str(), 0, posY, color );
			posY += 14;

			loopStep++;
		}

		for (PMENUITEM item : items) // DRAW ITEMS
		{
			DWORD color = item->color;

			if (item->isEnabled && loopStep == step) {
				color = D3DCOLOR_ARGB(fade, 0, 255, 0);
			} else if (item->isEnabled) {
				color = D3DCOLOR_ARGB(255, 0, 255, 0);
			}

			string text = item->itemName;

			if (item->value != nullptr) {
				switch (item->type)
				{
					case VAR_TYPE::T_INT:
					{
						text += "<" + to_string(*(INT*)item->value) + ">";
						break;
					}
					case VAR_TYPE::T_FLOAT:
					{
						text += "<" + to_string(*(FLOAT*)item->value) + ">";
						break;
					}
					case VAR_TYPE::T_STRING:
					{
						text += "<" + *(string*)item->value + ">";
						break;
					}
					default:
					{
						continue;
					}
				}
			} else {
				text += "<>";
			}


			rMGR->drawText(item->itemName.c_str(), 0, posY, color);

			posY += 14;
			loopStep++;
		}
	}

	if ((fade + 5) > 255)
	{
		fadeOut = TRUE;
	} else if ( (fade - 5) <= 0 ) {
		fadeOut = FALSE;
	}


	if (fadeOut)
	{
		fade -= 5;
	} else {
		fade += 5;
	}

	return VOID();
}

VOID D3Menu::handleInput()
{
	if (GetAsyncKeyState(VK_OEM_3) & 1)
	{
		isOpen = !isOpen;
	}

	if( !isOpen )
		return VOID();

	if (GetAsyncKeyState(VK_DOWN) & 1) // STEP DOWN
	{
		if( step == ( menus.size() + items.size() - 1) )
			step = 0;
		else { step++; }

	} else if (GetAsyncKeyState(VK_UP) & 1) // STEP UP
	{
		if( step == 0 )
			step = ( menus.size() + items.size() - 1);
		else { step--; }
	}

	if (GetAsyncKeyState(VK_RIGHT) & 1)
	{
		if (step < menus.size())
		{
			PMENUENTRY entry	= menus[step];

			entry->isOpen		= true;
			currentEntry		= entry;
		} else {
			PMENUITEM item		= items[step];
			
			if (item->value != nullptr)
			{
				switch (item->type)
				{
				case VAR_TYPE::T_INT:
					{
						if (item->stepCallback != nullptr)
						{
							stepCallback func = (stepCallback)item->stepCallback;
							func( item->value, 1 );
							break;
						}

						*(INT*)item->value += item->increment;
						break;
					}
				case VAR_TYPE::T_FLOAT:
					{
						if (item->stepCallback != nullptr)
						{
							stepCallback func = (stepCallback)item->stepCallback;
							func(item->value, 1);
							break;
						}

						*(FLOAT*)item->value += item->increment;
						break;
					}
				case VAR_TYPE::T_STRING:
					{
						if (item->stepCallback != nullptr)
						{
							callbackString func = (callbackString)item->stepCallback;
							func( &item->itemValue, (string*)item->value, 1 );
						}
						break;
					}
					default:
					{

					}
				}
			}
		}
	} else if (GetAsyncKeyState(VK_LEFT) & 1)
	{
		if (step < menus.size())
		{
			PMENUENTRY entry = menus[step];

			entry->isOpen	= true;
			currentEntry	= entry;
		} else {
			PMENUITEM item = items[step];

			if (item->value != nullptr)
			{
				switch (item->type)
				{
				case VAR_TYPE::T_INT:
					{
						if (item->stepCallback != nullptr)
						{
							stepCallback func = (stepCallback)item->stepCallback;
							func(item->value, -1 );
							break;
						}

						*(INT*)item->value -= item->increment;
						break;
					}
				case VAR_TYPE::T_FLOAT:
					{
						if (item->stepCallback != nullptr)
						{
							stepCallback func = (stepCallback)item->stepCallback;
							func(item->value, -1);
							break;
						}

						*(FLOAT*)item->value -= item->increment;
						break;
					}
				case VAR_TYPE::T_STRING:
					{
						if (item->stepCallback != nullptr)
						{
							callbackString func = (callbackString)item->stepCallback;
							func(&item->itemValue, (string*)item->value, -1);
						}
						break;
					}
					default:
					{

					}
				}
			}
		}
	}


	if (GetAsyncKeyState(VK_RETURN) & 1)
	{
		if (step < menus.size())
		{
			PMENUENTRY entry = menus[step];

			entry->isOpen = true;
			currentEntry = entry;
		} else {
			PMENUITEM item = items[step];

			if (item->callback != nullptr)
			{
				callback func = (callback)item->callback;
				func( item->value );
			}
		}
	}

	return VOID();
}

PMENUITEM D3Menu::createItem(string itemName, LPVOID value, DWORD color,SIZE_T increment, VAR_TYPE type, LPVOID callback)
{
	PMENUITEM item		= new MENUITEM;

	item->itemName	= itemName;
	item->value		= value;
	item->color		= color;
	item->increment	= increment;
	item->type		= type;
	item->callback	= callback;
	item->isEnabled	= FALSE;
	item->itemValue	= 0;

	items.push_back( item );
	return item;
}

PMENUITEM D3Menu::createItem(string menuName, string itemName, LPVOID value, DWORD color,SIZE_T increment, VAR_TYPE type, LPVOID callback )
{
	PMENUITEM item		= new MENUITEM;

	item->itemName	= itemName;
	item->value		= value;
	item->color		= color;
	item->increment	= increment;
	item->type		= type;
	item->callback	= callback;
	item->isEnabled	= FALSE;
	item->itemValue	= 0;

	for (PMENUENTRY entry : menus)
	{
		if (!menuName.compare(entry->entryName))
		{
			entry->items.push_back( item );
			return item;
		}
	}

	return nullptr;
}

PMENUITEM D3Menu::createItem(PMENUENTRY menu, string itemName, LPVOID value, DWORD color,SIZE_T increment, VAR_TYPE type, LPVOID callback)
{
	PMENUITEM item		= new MENUITEM;

	item->itemName	= itemName;
	item->value		= value;
	item->color		= color;
	item->increment	= increment;
	item->type		= type;
	item->callback	= callback;
	item->isEnabled	= FALSE;
	item->itemValue	= 0;

	if (menu == nullptr)
		return nullptr;

	menu->items.push_back( item );
	return item;
}

PMENUENTRY D3Menu::createMenu(string menuName)
{
	PMENUENTRY entry	= new MENUENTRY;

	entry->entryName		= menuName;
	entry->color			= D3DCOLOR_ARGB( 255, 0, 255, 0 );
	entry->isOpen		= false;

	menus.push_back( entry );
	return entry;
}

PMENUITEM D3Menu::getItem(string itemName)
{
	for (PMENUITEM item : items)
	{
		if (!itemName.compare(item->itemName))
		{
			return item;
		}
	}

	// IF ITEM WASN'T FOUND IN STANDART ITEMS, LOOK FOR IT IN ENTRIES
	for (PMENUENTRY entry : menus)
	{
		for (PMENUITEM item : entry->items)
		{
			if (!itemName.compare(item->itemName))
			{
				return item;
			}
		}
	}

	return nullptr;
}

PMENUITEM D3Menu::getItem(string menuName, string itemName)
{
	for (PMENUENTRY entry : menus)
	{
		if (!menuName.compare(entry->entryName))
		{
			for (PMENUITEM item : entry->items)
			{
				if (!itemName.compare(item->itemName))
				{
					return item;
				}
			}
		}
	}

	return nullptr;
}

PMENUITEM D3Menu::getItem(PMENUENTRY menu, string itemName)
{
	if (menu == nullptr)
		return nullptr;

	for (PMENUITEM item : menu->items)
	{
		if (!itemName.compare(item->itemName))
		{
			return item;
		}
	}

	return nullptr;
}

bool D3Menu::isItemEnabled(string itemName)
{
	for (PMENUITEM item : items)
	{
		if (!itemName.compare(item->itemName))
		{
			return item->isEnabled;
		}
	}

	// IF ITEM WASN'T FOUND IN STANDART ITEMS, LOOK FOR IT IN ENTRIES
	for (PMENUENTRY entry : menus)
	{
		for (PMENUITEM item : entry->items)
		{
			if (!itemName.compare(item->itemName))
			{
				return item->isEnabled;
			}
		}
	}

	return false;
}

bool D3Menu::isItemEnabled(string menuName, string itemName)
{
	for (PMENUENTRY entry : menus)
	{
		if (!menuName.compare(entry->entryName))
		{
			for (PMENUITEM item : entry->items)
			{
				if (!itemName.compare(item->itemName))
				{
					return item->isEnabled;
				}
			}
		}
	}

	return false;
}

bool D3Menu::isItemEnabled(PMENUENTRY menu, string itemName)
{
	if (menu == nullptr)
		return false;

	for (PMENUITEM item : menu->items)
	{
		if (!itemName.compare(item->itemName))
		{
			return item->isEnabled;
		}
	}

	return false;
}

LPVOID D3Menu::getItemValue(string itemName)
{
	for (PMENUITEM item : items)
	{
		if (!itemName.compare(item->itemName))
		{
			return item->value;
		}
	}

	// IF ITEM WASN'T FOUND IN STANDART ITEMS, LOOK FOR IT IN ENTRIES
	for (PMENUENTRY entry : menus)
	{
		for (PMENUITEM item : entry->items)
		{
			if (!itemName.compare(item->itemName))
			{
				return item->value;
			}
		}
	}

	return nullptr;
}

LPVOID D3Menu::getItemValue(string menuName, string itemName)
{
	for (PMENUENTRY entry : menus)
	{
		if (!menuName.compare(entry->entryName))
		{
			for (PMENUITEM item : entry->items)
			{
				if (!itemName.compare(item->itemName))
				{
					return item->value;
				}
			}
		}
	}

	return nullptr;
}

LPVOID D3Menu::getItemValue(PMENUENTRY menu, string itemName)
{
	if (menu == nullptr)
		return nullptr;

	for (PMENUITEM item : menu->items)
	{
		if (!itemName.compare(item->itemName))
		{
			return item->value;
		}
	}

	return nullptr;
}

VOID D3Menu::setItemValue(string itemName, LPVOID value)
{
	for (PMENUITEM item : items)
	{
		if (!itemName.compare(item->itemName))
		{
			item->value = value;
		}
	}

	// IF ITEM WASN'T FOUND IN STANDART ITEMS, LOOK FOR IT IN ENTRIES
	for (PMENUENTRY entry : menus)
	{
		for (PMENUITEM item : entry->items)
		{
			if (!itemName.compare(item->itemName))
			{
				item->value = value;
			}
		}
	}

}

VOID D3Menu::setItemValue(string menuName, string itemName, LPVOID value)
{
	for (PMENUENTRY entry : menus)
	{
		if (!menuName.compare(entry->entryName))
		{
			for (PMENUITEM item : entry->items)
			{
				if (!itemName.compare(item->itemName))
				{
					item->value = value;
				}
			}
		}
	}
}

VOID D3Menu::setItemValue(PMENUENTRY menu, string itemName, LPVOID value)
{
	if( menu == nullptr )
		return VOID();

	for (PMENUITEM item : menu->items)
	{
		if (!itemName.compare(item->itemName))
		{
			item->value = value;
		}
	}
}
