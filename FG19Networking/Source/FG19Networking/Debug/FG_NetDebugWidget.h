// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Blueprint/UserWidget.h"
#include "FG_NetDebugWidget.generated.h"


USTRUCT(BlueprintType)
struct FFGBlueprintNetworkSimulationSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network Settings", Meta = (DisplayName = "Minumum Latency", ClampMin = "0", ClampMax = "5000"))
	int32 MinLatency = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network Settings", Meta = (DisplayName = "Maximum Latency", ClampMin = "0", ClampMax = "5000"))
	int32 MaxLatency = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network Settings", Meta = (ClampMin = "0", ClampMax = "100"))
	int32 PacketLossPercentage = 0;
};

USTRUCT(BlueprintType)
struct FFGBlueprintNetworkSimulationSettingsText
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category = "Network Settings", Meta = (DisplayName = "Minimum Latency"))
	FText MinLatency;	  

	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category = "Network Settings", Meta = (DisplayName = "Maximum Latency"))
	FText MaxLatency;	 

	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category = "Network Settings")
	FText PacketLosspercentage;
};

UCLASS()
class FG19NETWORKING_API UFG_NetDebugWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = Widget)
	void UpdateNetworkSimulationSettings(const FFGBlueprintNetworkSimulationSettings& InPackets);

	UFUNCTION(BlueprintImplementableEvent, Category = Widget, meta = (DisplayName = "On Update Network Simulations Settings"))
	void BP_OnUpdateNetworkSimulationSettings(const FFGBlueprintNetworkSimulationSettingsText& InPackets);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category = Widget, meta = (DisplayName = "On Update Ping"))
	void BP_UpdatePing(int32 Ping);

	UFUNCTION(BlueprintImplementableEvent, Category = Widget, meta = (DisplayName = "On Show Widget"))
	void BP_OnShowWidget();

	UFUNCTION(BlueprintImplementableEvent, Category = Widget, meta = (DisplayName = "On Hide Widget"))
	void BP_OnHideWidget();
};
