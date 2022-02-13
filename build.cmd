@echo off
echo "Building URLAndFileLauncher for MiXTools..."
call build-debug.cmd
call build-prod.cmd
echo "Done building project!"
exit /b 0