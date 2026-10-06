# HelixMesh

HelixMesh is a separate payment-infrastructure concept I have been developing around offline-first connectivity for the Nigerian market.

## Concept

Merchant/payment nodes communicate locally over a LoRa mesh and use a gateway with internet connectivity to relay transactions to the wider payment network.

## Proposed architecture

Merchant nodes -> LoRa mesh -> HelixMesh gateway -> Starlink/internet -> payment network

The concept also considers battery-powered merchant boxes, existing phones through Bluetooth, local communication when internet access is unavailable and gateway connectivity when the backhaul becomes available.

This section is for design, research and prototype work. It is not presented as a deployed payment network.