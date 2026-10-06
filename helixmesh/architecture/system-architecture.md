# HelixMesh System Architecture

## Data Path
Phone -> BLE -> Merchant Node -> LoRa Relay -> Gateway -> Starlink/Internet -> Payment Processor

## Transaction Flow
1. Create transaction instruction locally.
2. Protect the transaction information.
3. Transfer from phone to merchant node over BLE.
4. Send through LoRa.
5. Relay nodes forward the packet.
6. Gateway receives it.
7. Gateway synchronizes with online payment infrastructure.
8. Regulated infrastructure performs processing and settlement.

## Boundary
LoRa is the communication transport, not the payment-settlement mechanism.
