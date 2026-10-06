# HelixMesh

## Offline-First Payment Relay Infrastructure

HelixMesh is a proposed resilient communication architecture for payment environments where conventional connectivity is unreliable.

## Architecture
Phone / Merchant -> BLE -> Merchant Node -> LoRa Mesh -> Gateway -> Starlink / Internet -> Payment Infrastructure

## Core Principle
HelixMesh does not perform payment settlement over LoRa. The mesh transports digitally protected transaction instructions toward an internet-connected gateway, which communicates with regulated payment infrastructure.

## Components
- ESP32-class merchant nodes
- BLE
- LoRa relay nodes
- Store-and-forward mesh
- Gateway
- Starlink/internet backhaul
- Payment integration

## Status
Architecture/prototype concept, not a deployed production payment network.
