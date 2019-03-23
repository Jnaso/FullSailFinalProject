#ifndef _UIMANAGER_H_
#define _UIMANAGER_H_

#pragma once

#include <Windows.h>
#include "graphicsclass.h"
#include "InputManager.h"

namespace UI
{
	struct UIType { enum TYPE { IMAGE, TEXT /*Add types if so needed*/ }; };

	class UIManager
	{
	private:

		
		Graphics* myGraphics;
		InputManager* myInput;

		struct UIComponent
		{
		private:

			Graphics* m_myGraphics;
			RECT m_dimensions;		//Dimensions Of Thing Being Drawing
			bool m_isInteractable;	//Determines if mouse position will be compared to objcec
			bool m_isMouseOver;		//Is True If Mouse Is Over (If isInteractable Is Enabled)
			const char* m_dir;		//Files Directory
			float2 m_position;		//Where the top left of the Image/Text will be(pixel based)
			const char* m_text;

		public:

			
			#pragma region Accessors & Mutators
			RECT GetDimensions()
			{
				return m_dimensions;
			}
			void SetDimensions(RECT r)
			{
				m_dimensions = r;
			}

			const char* GetDirectory()
			{
				return m_dir;
			}
			void SetDirectory(const char* dir)
			{
				m_dir = dir;
			}
			#pragma endregion

			UIComponent(Graphics* graphics, RECT dimensions, bool isInteractable, const char* filePath, float2 pos)
			{
				m_myGraphics =		graphics;			//Graphics Class Refrence
				m_dimensions =		dimensions;			//RECT refrence
				m_isInteractable = isInteractable;		//Interactable UI Refrence
				m_dir =				filePath;			//File Path Refrence
				m_position =		pos;				//Position Reference

				RECT defaultRect;
				defaultRect.top = 1;
				defaultRect.left = 1;
				defaultRect.right = 1;
				defaultRect.bottom = 1;

				if (dimensions == defaultRect)
				{
					m_myGraphics->CreateImage(m_dir, pos);
				}
				else
				{
					m_myGraphics->CreateImage(dimensions, m_dir, pos);
				}
			}

			UIComponent(Graphics* graphics, bool isInteractable, int font, const char* text, float2 pos)
			{
				m_myGraphics =			graphics;			//Graphics Class Refrence
				m_dimensions =			RECT{ 1,1,1,1 };	//RECT refrence
				m_isInteractable =		isInteractable;		//Interactable UI Refrence
				m_dir =					nullptr;			//File Path Refrence
				m_position =			pos;				//Position Reference
				m_text =				text;				//Text Refrence
				
				m_myGraphics->CreateText(text, font, pos);
			}


			~UIComponent()
			{

			}

		};


	public:

		std::vector<UIComponent> UIElements;

		UIManager(Graphics* graphics, InputManager* input);

		void CreateImage(RECT dimensions, bool interact, const char* filePath, float2 pos);

		void CreateText(const char* text, bool interact, int font, float2 pos);

		void Update();

		~UIManager();
	};
}

#endif