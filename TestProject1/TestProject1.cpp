#include<iostream>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<epm/epm.h>
#include<epm/epm_task_template_itk.h>
#include<fclasses/tc_string.h>
#include<tccore/aom_prop.h>

using namespace std;
#define DLLAPI _declspec(dllexport)
#define PLM_error (EMH_USER_error_base+6)
int ifail = ITK_ok;
char* cerror = NULL;

void null_check(tag_t tag)
{
	if (tag == NULL || tag == NULLTAG)
	{
		EMH_ask_error_text(ifail, &cerror);
		cout << cerror << endl;
		exit(0);
	}
}
void ifailcheck(int ifail)
{
	if (ifail != ITK_ok)
	{
		EMH_ask_error_text(ifail, &cerror);
		cout << cerror << endl;
		exit(0);
	}
}
void char_null_check(char value)
{
	if (value == NULL)
	{
		EMH_ask_error_text(ifail, &cerror);
		cout << cerror << endl;
		exit(0);
	}
}
extern "C"
{
	extern DLLAPI int TestProject1_register_callbacks();
	extern DLLAPI int handler_execute_callback1(int* decision, va_list argv);
	extern DLLAPI int add_custom_status(EPM_action_message_t* msg);
	extern DLLAPI int TestProject1_register_callbacks()
	{
		ifail = CUSTOM_register_exit("TestProject1", "USER_init_module", (CUSTOM_EXIT_ftn_t)handler_execute_callback1);
		cout << "Registering DLL :" << endl;
		return ifail;
	}
	extern DLLAPI int handler_execute_callback1(int* decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		cout << "Login successfull :" << endl;
		ifail = EPM_register_action_handler("Generate_QRCode", "Applying custom status :", (EPM_action_handler_t)add_custom_status);
		ifailcheck(ifail);
		cout << "Handler got registered successfully :" << endl;
		return ifail;
	}
	extern DLLAPI int add_custom_status(EPM_action_message_t* msg)
	{
		cout << "Entered in action :" << endl;
		tag_t root_task = NULLTAG;
		tag_t* attachments = NULL;
		tag_t classId = NULLTAG;
		char* className = NULL;
		tag_t trelease_status = NULLTAG;
		int attachment_count = 0;
		int count = 1;
		logical retained_date = true;
		char* sItemID = NULL;

		ifail = EPM_ask_root_task(msg->task, &root_task);
		ifailcheck(ifail);
		null_check(root_task);
		cout << "Root Task found :" << endl;
		ifail = EPM_ask_attachments(root_task, EPM_target_attachment, &attachment_count, &attachments);
		ifailcheck(ifail);
		null_check(*attachments);
		cout << "Attachments found :" << endl;
		cout << "Attachment count :" << endl;

		ifail = AOM_ask_value_string(attachments[0], "item_id", &sItemID);
		TC_write_syslog("Item ID: %s\n", sItemID);
		char str[512] = "\"C:\\Users\\13543\\Documents\\Inception - 2024\\Stage1\\TC_QR_Code_Project 2\\TC_QR_Code_Project\\x64\\Debug\\TC_QR_Code_Project.exe\"";
		const char* user = "-u=infodba";
		const char* password = "-p=infodba";
		const char* group = "-g=dba";

		snprintf(str + strlen(str), sizeof(str) - strlen(str), " %s %s %s -id=%s", user, password, group, sItemID);

		TC_write_syslog("%s\n", str);
		ifail = system(str);
		if (ifail == ITK_ok)
		{
			cout << "Release status has been added :" << endl;
		}
		else
		{
			EMH_store_error(EMH_severity_error, PLM_error);
			return PLM_error;
		}
		return ifail;
	}
}