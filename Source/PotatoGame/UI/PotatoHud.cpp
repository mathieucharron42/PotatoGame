#include "PotatoHud.h"

#include "PotatoGame/Gameplay/PotatoPlayerController.h"
#include "PotatoGame/UI/PotatoCharacterWidget.h"

void APotatoHud::BeginPlay()
{
	_potatoCharacterWidget = CreateWidget<UPotatoCharacterWidget>(GetWorld(), _potatoCharacterWidgetClass);
	if (ensure(_potatoCharacterWidget))
	{
		_potatoCharacterWidget->AddToViewport();
		APotatoPlayerController* controller = Cast<APotatoPlayerController>(GetOwningPlayerController());
		if (ensure(controller))
		{
			_potatoCharacterWidget->SetTargetPlayer(controller);
		}
	}
}
