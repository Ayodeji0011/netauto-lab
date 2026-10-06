# MikroTik + Starlink + Wireless Hotspot Network

A hands-on network deployment combining Starlink internet, MikroTik routing, a managed LAN, and TP-Link CPE wireless distribution.

## Network Architecture

Starlink
   |
MikroTik Router
   |
Switch
   |
+-- TP-Link CPE220
+-- TP-Link CPE220
+-- TP-Link CPE220

The MikroTik handled the local network and hotspot functions, while the TP-Link CPE devices were used for wireless distribution.

## Work Covered

- Starlink internet integration
- MikroTik router configuration
- LAN/bridge configuration
- IP addressing
- DHCP/network pool configuration
- Hotspot/captive portal setup
- Wireless access-point distribution
- TP-Link CPE220 configuration
- Network troubleshooting
- Client connectivity testing
- Captive-portal voucher configuration

## Hotspot

The network was configured around a MikroTik hotspot with voucher-based access.

The intended billing model was data-based rather than time-based, so users could be assigned a specific amount of usable data instead of simply receiving access for a fixed number of hours.

## Troubleshooting

One of the practical issues encountered during deployment was getting the wireless access point connected through the intended MikroTik interface while maintaining the correct bridge and hotspot behaviour.

This required checking:

- Bridge membership
- Interface configuration
- DHCP behaviour
- Hotspot binding
- IP addressing
- Wireless CPE configuration
- Client connectivity

## Skills Demonstrated

- MikroTik RouterOS
- Starlink networking
- Hotspot/captive portal
- DHCP
- Bridging
- IP addressing
- Wireless networking
- TP-Link CPE configuration
- Network troubleshooting

## Note

Private IP addresses, passwords, voucher credentials and other deployment-specific credentials are intentionally excluded from this repository.
