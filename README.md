DirectoryStructure.tree

controller
    android/
        build.sh
        bin/      # executable files go here, android apps have .apk extension (see notes below)
        xml/
            AndroidManifest.xml
            UdpSeender.xml
        src/
            cpp/
                android.mk          # Makefile
                UdpSender.cpp
                main.cpp
            py/
                recv.py
            java/
                MainActivity.java
                UdpSender.java
        inc/
        lib/
        notes/
            ALooper.notes
            CoordinateSystem-Android.md
            BuildingSDK.md
            KeyConcepts.md
            DirectoryStructure.tree 
