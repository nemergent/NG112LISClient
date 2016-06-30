Nemergent LIS Client
===============

* Author: Iñigo Ruiz Relloso
* Email:  iruizr7@gmail.com
* Copyright (c) 2016, Nemergent Initiative http://nemergent.com


This is a basic client supporting the [HELD (RFC 5985)][held] protocol, a small subset of [GEOPRIV (RFC 5941)][geopriv] embeded location information format, the [HELD Device Identity extensions (RFC 6155)][heldid] and the [Flow Identity Extension for HTTP-Enabled Location Delivery (HELD)][heldflow]

Build
-----
This LIS client depends on

* libxml2
* curl
* cmake

To build the client simply run the provided script:

	./make.sh

The LIS Client will be built in a `build` subfolder.

Usage
-----
	LISClient --locReq --url=<HELDURL> [--types=<locTypes> --exact --verbose]
	LISClient --locReq --url=<HELDURL> [--types=<locTypes> --exact --verbose] [--ip=<DEVICEIP> | --uri=<SIPURI> | --flow=<FLOW>]
	LISClient --locDeRef --url=<REFURL> [--types=<locTypes> --method=<POST|GET> --verbose]
	LISCLient --help
	LISCLient --version

Options
-------
	--locReq                        Perform a Location Request
	--url HELDURL                   Specifies a HELD URL of the LIS Server
	--types LOCTYPES                Specifies a series of Location Types to request, separated by commas.
	--exact                         Request exactly the specified location types.
	--ip IP_V4                      Perform a 3rd Party request, using the IP as the ID
	--uri sip:user@ip               Perform a 3rd Party request, using an URI as the ID (SIP, Tel...)

	--flow OIP:OPRT-DIP:DPRT/L3/L4  Perform a 3rd Party request, using a flow (RFC 6915) as the ID

	--locDeRef                      Perform a Location Dereference using a reference URL
	--method POST|GET               Specifies the HTTP method to perform the dereference request.

	--verbose                       Makes the program output more debug info.


	--help                          Show this screen.
	--version                       Show version.


[held]: https://tools.ietf.org/html/rfc5985 "HTTP-Enabled Location Delivery (HELD)"
[heldid]: https://tools.ietf.org/html/rfc6155 "Use of Device Identity in HTTP-Enabled Location Delivery (HELD)"
[geopriv]: https://tools.ietf.org/html/rfc5941 "GEOPRIV Presence Information Data Format Location Object (PIDF-LO) Usage Clarification, Considerations, and Recommendations"
[heldflow]: https://tools.ietf.org/html/rfc6915 "Flow Identity Extension for HTTP-Enabled Location Delivery (HELD)"
