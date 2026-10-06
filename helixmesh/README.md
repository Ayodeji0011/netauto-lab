# HelixMesh

An offline-first payment relay infrastructure concept designed for areas with unreliable conventional internet connectivity.

## Concept

Merchant devices communicate locally with nearby relay nodes. Transaction instructions can then travel through a LoRa mesh to a gateway with internet connectivity.

The gateway can synchronize transactions through an available backhaul such as Starlink.

## Architecture

Merchant Phone
      |
     BLE
      |
LoRa Relay Mesh
      |
HelixMesh Gateway
      |
Starlink / Internet
      |
Payment Infrastructure

## Technology Areas

- ESP32
- LoRa
- BLE
- Starlink
- Mesh networking
- Offline-first systems
- Payment infrastructure
- API integration

## Important Design Principle

HelixMesh is intended as a communication and transaction-relay infrastructure.

The LoRa mesh does not directly move or settle money. Transaction instructions are transported through the mesh and ultimately synchronized with regulated payment infrastructure.

## Status

Concept, architecture and prototype design work.
