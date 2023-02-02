#pragma once

#include <iostream>
#include <chrono>
#include <numbers>
#include "Application.h"

constexpr float CLOCK_RADIUS = 200.f;

constexpr float HOURS_NEEDLE_LENGTH = CLOCK_RADIUS * .5f;
constexpr float MINUTES_NEEDLE_LENGTH = CLOCK_RADIUS * .8f;
constexpr float SECONDS_NEEDLE_LENGTH = CLOCK_RADIUS * .9f;

constexpr float HOURS_NEEDLE_THICKNESS = 3.f;
constexpr float MINUTES_NEEDLE_THICKNESS = 2.5f;
constexpr float SECONDS_NEEDLE_THICKNESS = .5f;

class Clock : public Application
{
public:
    Clock() = default;
    ~Clock() override = default;

    void Startup() override {}

    void Update() override
    {
        auto now = std::chrono::system_clock::now();
        auto timeOfTheDay = now - std::chrono::floor<std::chrono::days>(now);
        std::chrono::hh_mm_ss hms{ timeOfTheDay };

        auto h = hms.hours();
        auto m = hms.minutes();
        auto s = hms.seconds();

        ImDrawList* draw = ImGui::GetForegroundDrawList();
        ImU32 color = ImGui::GetColorU32(ImGuiCol_Text);
        float windowsWidth = ImGui::GetIO().DisplaySize.x;
        float windowsHeight = ImGui::GetIO().DisplaySize.y;
        center_ = ImVec2(
            windowsWidth / 2,
            windowsHeight / 2
        );

        float secondsRatio = s.count() / 60.f;
        float secondsAngle = secondsRatio * 2 * std::numbers::pi - std::numbers::pi / 2;

        float minutesRatio = ( m.count() + secondsRatio ) / 60.f;
        float minutesAngle = minutesRatio * 2 * std::numbers::pi - std::numbers::pi / 2;

        float hoursRatio = ( h.count() + minutesRatio ) / 12.f;
        float hoursAngle = hoursRatio * 2 * std::numbers::pi - std::numbers::pi / 2;

        // Clockwise direction
        secondsAngle *= -1;
        minutesAngle *= -1;
        hoursAngle *= -1;

        DrawClock(draw, color);
        DrawSecondsNeedle(draw, secondsAngle, color);
        DrawMinutesNeedle(draw, minutesAngle, color);
        DrawHoursNeedle(draw, hoursAngle, color);

    }

private:
    ImVec2 center_;

    void DrawClock(ImDrawList* draw, ImU32 color) const
    {
        draw->AddCircle(center_, CLOCK_RADIUS, color);
        for (int hour = 0; hour < 12; hour++) 
        {
            float angle = hour / 12.f * 2 * std::numbers::pi - std::numbers::pi / 2;
            ImVec2 start(
                center_.x + std::cos(angle) * ( CLOCK_RADIUS - 10.f ),
                center_.y - std::sin(angle) * ( CLOCK_RADIUS - 10.f )
            );
            DrawLineWithAngle(draw, start, 20.f, angle, color, .3f);
        }
        draw->AddCircleFilled(center_, 5.f, color);
    }
    void DrawLineWithAngle(ImDrawList* draw, const ImVec2& start, float length, float angle, ImU32 color, float thickness = 1.f) const
    {
        draw->AddLine(
            start,
            ImVec2(
                start.x + std::cos( angle ) * length,
                start.y - std::sin( angle ) * length
            ),
            color,
            thickness
        );
    }
    
    void DrawHoursNeedle(ImDrawList* draw, float angle, ImU32 color) const
    {
        DrawLineWithAngle(draw, center_, HOURS_NEEDLE_LENGTH, angle, color, HOURS_NEEDLE_THICKNESS);
    }
    void DrawMinutesNeedle(ImDrawList* draw, float angle, ImU32 color) const
    {
        DrawLineWithAngle(draw, center_, MINUTES_NEEDLE_LENGTH, angle, color, MINUTES_NEEDLE_THICKNESS);
    }
    void DrawSecondsNeedle(ImDrawList* draw, float angle, ImU32 color) const
    {
        DrawLineWithAngle(draw, center_, SECONDS_NEEDLE_LENGTH, angle, color, SECONDS_NEEDLE_THICKNESS);
    }
};