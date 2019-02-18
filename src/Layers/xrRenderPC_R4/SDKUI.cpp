#include "stdafx.h"
#include "SDKUI.h"
#include "../../xrCore/Imgui/imgui.h"
#include "../../xrCore/Imgui/imgui_impl_sdl.h"
#include "../../xrCore/Imgui/imgui_impl_dx11.h"
#include "xrEngine/XR_IOConsole.h"

void SDKUI::Begin(void)
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplSDL2_NewFrame(Device.m_sdlWnd);
    ImGui::NewFrame();
}

void SDKUI::KeyBoardMessages(void)
{
    // @ Хендлим шорткаты

    // @ Глобальные клики можно обрабатывать динамические пупапы если надо
    if (ImGui::IsMouseClicked(0)) // @ Left button
    {

    }

    if (ImGui::IsMouseClicked(1)) // @ Right button
    {

    }

    if (ImGui::IsMouseClicked(2)) // @ Wheel
    {

    }

    // @ Exit
    if (ImGui::IsKeyDown(SDL_Scancode::SDL_SCANCODE_LALT) || ImGui::IsKeyDown(SDL_Scancode::SDL_SCANCODE_RALT))
    {
        if (ImGui::IsKeyPressed(SDL_Scancode::SDL_SCANCODE_F4) && !bCloseOnce)
        {
            SDKUI_Log::Widget().SetColor(good);
            SDKUI_Log::Widget().AddText("Application is closing...");
            Console->Execute("quit");
            bCloseOnce = true;
            return;
        }
    }

    // @ Screenshot
    if (ImGui::IsKeyDown(SDL_Scancode::SDL_SCANCODE_F12))
    {
        Console->Execute("screenshot 1");
        SDKUI_Log::Widget().AddText("Screenshot was made!", SDKErrorType::special);
        return;
    }

    // @ Log
    if (ImGui::IsKeyDown(SDL_Scancode::SDL_SCANCODE_LCTRL) || ImGui::IsKeyDown(SDL_Scancode::SDL_SCANCODE_RCTRL))
    {
        if (ImGui::IsKeyPressed(SDL_Scancode::SDL_SCANCODE_L))
        {
            if (!SDKUI_Log::Widget().GetVisible())
            {
                SDKUI_Log::Widget().SetColor(unimportant);
                SDKUI_Log::Widget().AddText("Log window is shown");
                SDKUI_Log::Widget().Show();
            }
        }
    }

    // @ Overlay
    if (ImGui::IsKeyDown(SDL_Scancode::SDL_SCANCODE_LCTRL) || ImGui::IsKeyDown(SDL_Scancode::SDL_SCANCODE_RCTRL))
    {
        if (ImGui::IsKeyDown(SDL_Scancode::SDL_SCANCODE_LALT) || ImGui::IsKeyDown(SDL_Scancode::SDL_SCANCODE_RALT))
        {
            if (ImGui::IsKeyPressed(SDL_Scancode::SDL_SCANCODE_S))
            {
                if (!SDKUI_Overlay::Widget().GetVisible())
                {
                    SDKUI_Overlay::Widget().Show();
                    SDKUI_Log::Widget().SetColor(unimportant);
                    SDKUI_Log::Widget().AddText("Overlay window is shown");
                }
            }
        }
    }
}

void SDKUI::DrawAllHelpers(void)
{
    SDKUI_Log::Widget().Draw();
    SDKUI_Overlay::Widget().Draw();
}

void SDKUI::DrawMainMenuBar(void)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Quit", "ALT+F4"))
            {
                SDKUI_Log::Widget().SetColor(good);
                SDKUI_Log::Widget().AddText("Application is closing...");
                Console->Execute("quit");
                bCloseOnce = true;
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Tools"))
        {
            if (ImGui::BeginMenu("Others"))
            {
                if (ImGui::MenuItem("Show Log", "CTRL+L"))
                {
                    if (SDKUI_Log::Widget().GetVisible() == false)
                    {
                        SDKUI_Log::Widget().Show();
                        SDKUI_Log::Widget().SetColor(unimportant);
                        SDKUI_Log::Widget().AddText("The logger show");
                    }
                }

                if (ImGui::MenuItem("Show Overlay", "CTRL+ALT+S"))
                {
                    SDKUI_Overlay::Widget().Show();
                    SDKUI_Log::Widget().SetColor(unimportant);
                    SDKUI_Log::Widget().AddText("Overlay window is shown");
                }

                if (ImGui::MenuItem("ScreenShot", "F12"))
                {
                    Console->Execute("screenshot 1");
                    SDKUI_Log::Widget().SetColor(special);
                    SDKUI_Log::Widget().AddText("Screenshot was made!");
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void SDKUI::Draw(void)
{
    ImGui::ShowDemoWindow();
    KeyBoardMessages();
    DrawMainMenuBar();
    DrawAllHelpers();

    ImGui::Render();
}

void SDKUI::End(void)
{
    if (ImGui::GetDrawData())
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

/*
    PRIVATE SECTION
*/
