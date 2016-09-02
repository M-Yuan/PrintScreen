@echo ==================================
@echo SET VisualStudio PATH
@echo ==================================
call "%VS110COMNTOOLS%\vsvars32.bat

@echo ==================================
@echo Clean VisualStudio Solution
@echo ==================================
devenv PrintScreen.sln /Clean Debug
devenv PrintScreen.sln /Clean Release
rmdir Debug /s /q
rmdir Release /s /q
rmdir PrintScreen\Debug /s /q
rmdir PrintScreen\Release /s /q
del PrintScreen\*.bmp
del *.sdf
del *.log
rmdir ipch /s /q

PAUSE