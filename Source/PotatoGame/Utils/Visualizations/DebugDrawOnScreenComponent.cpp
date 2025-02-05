#include "DebugDrawOnScreenComponent.h"

#include "DrawDebugHelpers.h"
#include "Engine/Canvas.h"
#include "GameFramework/HUD.h"

static TAutoConsoleVariable<int32> CVarEnableDebugDrawOnScreen(
	TEXT("potato.DebugDrawOnScreen"),
	1,
	TEXT("Enables debug draw on screen components")
);

void UDebugDrawOnScreenComponent::EnableDrawLine(const FDrawOnScreenLineProperties& properties)
{
	_lineEnabled = true;
	_lineProperties = properties;
}

void UDebugDrawOnScreenComponent::EnableDrawBox(const FDrawOnScreenBoxProperties& properties)
{
	_boxEnabled = true;
	_boxProperties = properties;
}

void UDebugDrawOnScreenComponent::EnableDrawCircle(const FDrawOnScreenCircleProperties& properties)
{
	_circleEnabled = true;
	_circleProperties = properties;
}

void UDebugDrawOnScreenComponent::EnableDrawText(const FDrawOnScreenTextProperties& properties)
{
	_textEnabled = true;
	_textProperties = properties;
}

void UDebugDrawOnScreenComponent::BeginPlay()
{
	Super::BeginPlay();
	AHUD::OnHUDPostRender.AddUObject(this, &UDebugDrawOnScreenComponent::OnPostRender);
}

void UDebugDrawOnScreenComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	AHUD::OnHUDPostRender.RemoveAll(this);
}

void UDebugDrawOnScreenComponent::OnPostRender(AHUD* HUD, UCanvas* canvas)
{
	AActor* a = GetOwner();
	if (CVarEnableDebugDrawOnScreen.GetValueOnGameThread())
	{
		if (IsValid(canvas)) 
		{
			if (_lineEnabled)
			{
				DrawDebugCanvas2DLine(canvas, _lineProperties.Start, _lineProperties.End, _lineProperties.Color, _lineProperties.Thickness);
			}

			if (_boxEnabled)
			{
				DrawDebugCanvas2DBox(canvas, _boxProperties.Box, _boxProperties.Color, _boxProperties.Thickness);
			}

			if (_circleEnabled)
			{
				DrawDebugCanvas2DCircle(canvas, _circleProperties.Center, _circleProperties.Radius, _circleProperties.NbSide, _circleProperties.Color, _circleProperties.Thickness);
			}

			if (_textEnabled)
			{
				FCanvasTextItem textItem(_textProperties.Position, _textProperties.Message, GEngine->GetMediumFont(), _textProperties.Color);
				textItem.Scale = _textProperties.Scale;
				canvas->DrawItem(textItem);
			}
		}
	}
}