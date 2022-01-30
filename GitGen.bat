git submodule add https://github.com/powerof3/CommonLibSSE.git ./external/CommonLibSSE
cd ./external/CommonLibSSE
git checkout dev
cd ../../

git submodule add https://github.com/Exit-9B/CommonLibSSE.git ./external/CommonLibAE

git submodule add https://github.com/alandtse/CommonLibVR.git ./external/CommonLibVR
cd ./external/CommonLibVR
git checkout vr
cd ../../

git submodule add https://github.com/Exit-9B/CommonLibSSE.git ./external/CommonLibAEVR
cd ./external/CommonLibAEVR
git checkout vr

pause