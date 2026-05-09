#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDebug>
#include <QString>
#include <QUrl>
#include <QWidget>

#include "models/profilemodel.h"

class Preferences : public QObject
{
    Q_OBJECT

  public:

    static Preferences *self();

    ~Preferences();

    void read();

    void save();

    void resetDefaults();

    void showDialog(QWidget* parent, ProfileModel* profileModel);

    static
    void setFirstRun( bool v )
    {
      if (!self()->isFirstRunImmutable())
        self()->mFirstRun = v;
    }

    static
    bool firstRun()
    {
      return self()->mFirstRun;
    }

    static
    bool isFirstRunImmutable()
    {
      return false;
    }

    static
    void setOverwriteExistingFiles( bool v )
    {
      if (!self()->isOverwriteExistingFilesImmutable())
        self()->mOverwriteExistingFiles = v;
    }

    static
    bool overwriteExistingFiles()
    {
      return self()->mOverwriteExistingFiles;
    }

    static
    bool isOverwriteExistingFilesImmutable()
    {
      return false;
    }

    static
    void setDeletePartialFiles( bool v )
    {
      if (!self()->isDeletePartialFilesImmutable())
        self()->mDeletePartialFiles = v;
    }

    static
    bool deletePartialFiles()
    {
      return self()->mDeletePartialFiles;
    }

    static
    bool isDeletePartialFilesImmutable()
    {
      return false;
    }

    static
    void setEjectCDTray( bool v )
    {
      if (!self()->isEjectCDTrayImmutable())
        self()->mEjectCDTray = v;
    }

    static
    bool ejectCDTray()
    {
      return self()->mEjectCDTray;
    }

    static
    bool isEjectCDTrayImmutable()
    {
      return false;
    }

    static
    void setCddbLookupAuto( bool v )
    {
      if (!self()->isCddbLookupAutoImmutable())
        self()->mCddbLookupAuto = v;
    }

    static
    bool cddbLookupAuto()
    {
      return self()->mCddbLookupAuto;
    }

    static
    bool isCddbLookupAutoImmutable()
    {
      return false;
    }

    static
    void setCoverLookupAuto( bool v )
    {
      if (!self()->isCoverLookupAutoImmutable())
        self()->mCoverLookupAuto = v;
    }

    static
    bool coverLookupAuto()
    {
      return self()->mCoverLookupAuto;
    }

    static
    bool isCoverLookupAutoImmutable()
    {
      return false;
    }

    static
    void setFetchCount( int v )
    {
      if (v < 1)
      {
        qDebug() << "setFetchCount: value " << v << " is less than the minimum value of 1";
        v = 1;
      }

      if (v > 99)
      {
        qDebug() << "setFetchCount: value " << v << " is greater than the maximum value of 99";
        v = 99;
      }

      if (!self()->isFetchCountImmutable())
        self()->mFetchCount = v;
    }

    static
    int fetchCount()
    {
      return self()->mFetchCount;
    }

    static
    bool isFetchCountImmutable()
    {
      return false;
    }

    static
    void setParanoiaMode( bool v )
    {
      if (!self()->isParanoiaModeImmutable())
        self()->mParanoiaMode = v;
    }

    static
    bool paranoiaMode()
    {
      return self()->mParanoiaMode;
    }

    static
    bool isParanoiaModeImmutable()
    {
      return false;
    }

    static
    void setNeverSkip( bool v )
    {
      if (!self()->isNeverSkipImmutable())
        self()->mNeverSkip = v;
    }

    static
    bool neverSkip()
    {
      return self()->mNeverSkip;
    }

    static
    bool isNeverSkipImmutable()
    {
      return false;
    }

    static
    void setSampleOffset( int v )
    {
      if (v < -10000)
      {
        qDebug() << "setSampleOffset: value " << v << " is less than the minimum value of -10000";
        v = -10000;
      }

      if (v > 10000)
      {
        qDebug() << "setSampleOffset: value " << v << " is greater than the maximum value of 10000";
        v = 10000;
      }

      if (!self()->isSampleOffsetImmutable())
        self()->mSampleOffset = v;
    }

    static
    int sampleOffset()
    {
      return self()->mSampleOffset;
    }

    static
    bool isSampleOffsetImmutable()
    {
      return false;
    }

    static
    void setBasePath( const QString & v )
    {
      if (!self()->isBasePathImmutable())
        self()->mBasePath = v;
    }

    static
    QString basePath()
    {
      return self()->mBasePath;
    }

    static
    bool isBasePathImmutable()
    {
      return false;
    }

    static
    void setGuiWidth( int v )
    {
      if (!self()->isGuiWidthImmutable())
        self()->mGuiWidth = v;
    }

    static
    int guiWidth()
    {
      return self()->mGuiWidth;
    }

    static
    bool isGuiWidthImmutable()
    {
      return false;
    }

    static
    void setGuiHeight( int v )
    {
      if (!self()->isGuiHeightImmutable())
        self()->mGuiHeight = v;
    }

    static
    int guiHeight()
    {
      return self()->mGuiHeight;
    }

    static
    bool isGuiHeightImmutable()
    {
      return false;
    }

    static
    void setGuiLayout( int v )
    {
      if (!self()->isGuiLayoutImmutable())
        self()->mGuiLayout = v;
    }

    static
    int guiLayout()
    {
      return self()->mGuiLayout;
    }

    static
    bool isGuiLayoutImmutable()
    {
      return false;
    }

    private:
    Preferences(QObject* parent = nullptr);
    friend class PreferencesHelper;


    // general
    bool mFirstRun;
    bool mOverwriteExistingFiles;
    bool mDeletePartialFiles;
    bool mEjectCDTray;
    bool mCddbLookupAuto;
    bool mCoverLookupAuto;
    int mFetchCount;
    bool mParanoiaMode;
    bool mNeverSkip;
    int mSampleOffset;
    QString mBasePath;
    int mGuiWidth;
    int mGuiHeight;
    int mGuiLayout;
};

#endif

