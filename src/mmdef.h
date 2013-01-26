#ifndef MMDEF_H
#define MMDEF_H
#include <QString>
#include <QObject>
#include <QStringList>

#ifndef TR
#define TR(TEXT) QObject::tr(TEXT)
#endif

#ifndef DECLARE_STRING
#define DECLARE_STRING(VAR,STRING) const QString VAR = TR(STRING);
#endif

namespace MM
{
    namespace Config
    {
        const QString CompanyName = "Doit";
        const QString ApplicationName = "Memory Monitor";
    }

    namespace Menu
    {

    }

    namespace ToolBar
    {

    }

    namespace Text
    {

        DECLARE_STRING(Menu_File,"File")
        DECLARE_STRING(File_Exit,"Exit")
        DECLARE_STRING(Menu_Config,"Configurations")
        DECLARE_STRING(Config_SelectColumn,"Select Columns")
        DECLARE_STRING(Menu_Window,"Window")
        DECLARE_STRING(Window_Log,"Query Log")
        DECLARE_STRING(Window_MemoryUtility,"Memory Utility")
        DECLARE_STRING(Window_WMIQuery,"WMI Query")
        DECLARE_STRING(Menu_About,"About")
        DECLARE_STRING(About_Help,"Help")
        DECLARE_STRING(About_This,"About this")

        DECLARE_STRING(ToolBar_Tools,"Tools")

        DECLARE_STRING(SelectColumn,"Select Columns")
        DECLARE_STRING(UpdateIntervalLabel,"Update Interval:")

        DECLARE_STRING(TB_StartQuery,"Start Query")
        DECLARE_STRING(TB_StopQuery,"Stop Query")
        DECLARE_STRING(TB_ClearHistoryData,"Clear History Data")
        DECLARE_STRING(TB_ShowPopupChart,"Show Popup Chart")

        DECLARE_STRING(MMW_RefreshProcess,"Refresh")
        DECLARE_STRING(MMW_StartMonitor,"Start Monitoring")
        DECLARE_STRING(MMW_StopMonitor,"Stop Monitoring")

        DECLARE_STRING(Title_MMW,"Memory Monitor Utility")
        DECLARE_STRING(Title_WMIQueryWindow,"WMI Query")
        DECLARE_STRING(Title_LogOutput,"Log Output")

        DECLARE_STRING(Label_ProcessList,"Process List:")
        DECLARE_STRING(Label_Columns,"Columns:")

        DECLARE_STRING(Info_AboutThis,"<h1>Memory Monitor</h1><p><b>Copyright by Kimtaikee 2012</b>")

        DECLARE_STRING(Status_ShowPopup,"show popup chart ...")
        DECLARE_STRING(Status_HidePopup,"hide popup chart ...")
        DECLARE_STRING(Status_ClearHistoryData,"clear history data ...")

        DECLARE_STRING(Empty,"")

        // qaction
        DECLARE_STRING(Action_Export,"Export")
    }


    namespace Constant
    {
        const int RowHeight = 20;
        const int DefaultRefreshInterval = 5000; // ms

        const QString IDProcess = "ProcessId";
        const QString Name = "Name";
        const QString ProcessHeaders = TR("Name;Usage;PID;Thread Count;WorkingSet");
        const QString CMD_Engine = "wmic.exe";
        const QString OutputForamt = "/format:csv"; // we use this format to ease the parsing procedure
        const QString ColumnSeparator = ",";
        const QString ProcessPerfClass = "win32_process";
        const QString TopSeparator = "==";
        const QStringList DefaultColumns = QStringList() << "Name" << "ProcessId" << "ThreadCount" << "ParentProcessID"
                                                         << "WorkingSetSize" << "ExecutablePath";
        const QStringList MustHaveColums = QStringList() << "Name" << "ProcessId";
        const QStringList MonitorColumns = QStringList() << "HandleCount" << "OtherTransferCount" << "OtherOperationCount"
                                                         << "PageFaults" << "PageFileUsage" << "PeakPageFileUsage"
                                                         << "PeakVirtualSize" << "PeakWorkingSetSize" << "PrivatePageCount"
                                                         << "ReadOperationCount" << "ReadTransferCount" << "VirtualSize"
                                                         << "WorkingSetSize" << "WriteOperationCount" << "WriteTransferCount";
        const QStringList MonitorRefreshColumns = QStringList() << "Name" << "ProcessID";


    }

}
#endif // MMDEF_H
