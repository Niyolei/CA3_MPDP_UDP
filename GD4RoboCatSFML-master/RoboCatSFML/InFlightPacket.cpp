//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
#include "RoboCatPCH.hpp"

InFlightPacket::InFlightPacket(PacketSequenceNumber inSequenceNumber) :
	mSequenceNumber(inSequenceNumber),
	mTimeDispatched(Timing::sInstance.GetTimef())
{
	//null out other transmision data params...
}


void InFlightPacket::HandleDeliveryFailure(DeliveryNotificationManager* inDeliveryNotificationManager) const
{
	for (const auto& pair : mTransmissionDataMap)
	{
		pair.second->HandleDeliveryFailure(inDeliveryNotificationManager);
	}
}

void InFlightPacket::HandleDeliverySuccess(DeliveryNotificationManager* inDeliveryNotificationManager) const
{
	for (const auto& pair : mTransmissionDataMap)
	{
		pair.second->HandleDeliverySuccess(inDeliveryNotificationManager);
	}
}
