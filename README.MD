Agora Electron RTC SDK takes advantage of NODEJS C++ ADDON and Agora native SDK.

===========================================================================================
Code structure:
	1. agora_node_ext : NODEJS C++ ADDON which wraps functionalities of Agora Native SDK.
	2. common : Utilities that used by agora_node_ext and video source
	3. video source : agora_node_ext implement multiple video sources functionality by using multiple processes. Video source provides the ancillary facilities.
	4. js : contains JS level API.
	5. sdk : Agora Native SDk, used to generate NODEJS ADDON.
	6. demon : one very simple demon.
	
===========================================================================================
Version

	Currently, latest Electron is 1.7.9, with 7.9.0 NODEJS support.
	NODEJS wraps Agora Native SDK V2.0.0
	
===========================================================================================
Build : 
	1. Install NODEJS and GYP.
	2. Run build.bat script. If MSVC version or Electron version is not the same as the script, change the corresponding parameters in the script.