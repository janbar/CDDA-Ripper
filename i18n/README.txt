### Update resource file

  lupdate ../mainwindow.cpp ../preferences.cpp ../core ../dialogs ../models ../utils ../widgets -ts cddaripper_en.ts
  lupdate ../mainwindow.cpp ../preferences.cpp ../core ../dialogs ../models ../utils ../widgets -ts cddaripper_fr.ts

### Release reviewed translations

  lrelease cddaripper_en.ts
  lrelease cddaripper_fr.ts

