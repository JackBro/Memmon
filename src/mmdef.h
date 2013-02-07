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

        DECLARE_STRING(File_Exit,"Exit")
        DECLARE_STRING(Config_SelectColumn,"Select Columns")
        DECLARE_STRING(Window_Log,"Query Log")
        DECLARE_STRING(Window_MemoryUtility,"Memory Utility")
        DECLARE_STRING(Window_WMIQuery,"WMI Query")
        DECLARE_STRING(About_Help,"Help")
        DECLARE_STRING(About_This,"About this")

        DECLARE_STRING(ToolBar_Tools,"Tools")

        DECLARE_STRING(SelectColumn,"Select Columns")
        DECLARE_STRING(Win32_Process,"Win32_Process")
        DECLARE_STRING(Win32_PerfFormattedDataPerfProc_Process,"Win32_PerfFormattedData_PerfProc_Process")
        DECLARE_STRING(UpdateIntervalLabel,"Update Interval:")

        DECLARE_STRING(TB_StartQuery,"Start Query")
        DECLARE_STRING(TB_StopQuery,"Stop Query")
        DECLARE_STRING(TB_ClearHistoryData,"Clear History Data")
        DECLARE_STRING(TB_ShowPopupChart,"Show Popup Chart")
        DECLARE_STRING(TB_GeneralInfo,"General Info")

        DECLARE_STRING(MMW_RefreshProcess,"Refresh")
        DECLARE_STRING(MMW_StartMonitor,"Start Monitoring")
        DECLARE_STRING(MMW_StopMonitor,"Stop Monitoring")

        DECLARE_STRING(Title_MMW,"Memory Monitor Utility")
        DECLARE_STRING(Title_WMIQueryWindow,"WMI Query")
        DECLARE_STRING(Title_LogOutput,"Log Output")

        // qlabel
        DECLARE_STRING(Label_ProcessList,"Process List:")
        DECLARE_STRING(Label_Columns,"Columns:")
        DECLARE_STRING(Label_UpdateInterval,"Update Interval:")

        DECLARE_STRING(Info_AboutThis,"<h1>Memory Monitor</h1><p><b>Copyright by Kimtaikee 2012</b>")

        DECLARE_STRING(Status_ShowPopup,"show popup chart ...")
        DECLARE_STRING(Status_HidePopup,"hide popup chart ...")
        DECLARE_STRING(Status_ClearHistoryData,"clear history data ...")

        DECLARE_STRING(Empty,"")

        // qaction
        DECLARE_STRING(Action_Export,"Export")
        DECLARE_STRING(Action_Exit,"Exit")
        DECLARE_STRING(Action_SelectColumn,"Select Columns")
        DECLARE_STRING(Action_MemoryUtility,"Memory Utility")
        DECLARE_STRING(Action_WmiQuery,"WMI Query")
        DECLARE_STRING(Action_QueryLog,"Query Log")
        DECLARE_STRING(Action_AboutThis,"About This")
        DECLARE_STRING(Action_Help,"Help")

        // qmenu
        DECLARE_STRING(Menu_File,"File")
        DECLARE_STRING(Menu_Config,"Configurations")
        DECLARE_STRING(Menu_SwitchQueryEngine,"Switch Query Engine")
        DECLARE_STRING(Menu_Window,"Window")
        DECLARE_STRING(Menu_About,"About")

        // qtoolbutton
        DECLARE_STRING(ToolButton_Start,"Start Query")
        DECLARE_STRING(ToolButton_Stop,"Stop Query")
        DECLARE_STRING(ToolButton_Clear,"Clear History Data")
        DECLARE_STRING(ToolButton_Popup,"Sow Popup Chart")

        // qcombobox

        // qtoolbar
        DECLARE_STRING(ToolBar_Tool,"Tool")

        // general info
        DECLARE_STRING(Info_AppName,"Memmon")
        DECLARE_STRING(Info_CorpName,"Doit Inc.")

        // registry keys
        DECLARE_STRING(Key_WindowStates,"WindowStates")
        DECLARE_STRING(Key_QueryEngine,"QueryEngine")
    }

    namespace Constant
    {
        const int RowHeight = 20;
        const int DefaultRefreshInterval = 5000; // ms
        const int ExtraSpace = 10;

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

    namespace Icon
    {
        DECLARE_STRING(Clear,":/images/clear.png")
        DECLARE_STRING(Save,":/images/save.png")
        DECLARE_STRING(Process,":/images/process.png")
        DECLARE_STRING(StartQuery,":/images/start_query.png")
        DECLARE_STRING(StopQuery,":/images/stop_query.png")
        DECLARE_STRING(Open,":/images/open.png")
        DECLARE_STRING(Pin,":/images/pin.png")
        DECLARE_STRING(Chart,":/images/chart.png")
        DECLARE_STRING(Close,":/images/close.png")
        DECLARE_STRING(GeneralInfo,":/images/info.png")
        DECLARE_STRING(ExportImage,":/images/export_image.png")
        DECLARE_STRING(SelectColumn,":/images/select_column.png")
    }

}
#endif // MMDEF_H
