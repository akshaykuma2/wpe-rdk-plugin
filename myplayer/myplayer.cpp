#include"myplayer.h"
#include"bits/stdc++.h"
#include <gst/gst.h>
#include <pthread.h>
using namespace std;

namespace WPEFramework
{
	namespace Plugin
	{
		
		SERVICE_REGISTRATION(myplayer,1,0);
		//service registration 
		myplayer::myplayer():PluginHost::JSONRPC()
		{
			Register<JsonObject, void>(_T("Path"),&myplayer::Path, this);
			Register<void, void>(_T("pause"), &myplayer::pause, this);
			Register<void, void>(_T("loop"),&myplayer::loop, this);
		}
		myplayer::~myplayer()
		//destoucter
		{
		}
		//initialzing plugin
		const string myplayer::Initialize(PluginHost::IShell* service )
		{
		 cout<<"\n plugin initialized"<<endl;
		 int argc=0;
		 char **argv=nullptr;
		 gst_init(&argc, &argv);
		 return (string());
		}
		//deinitializing the plugin if the framework is exiting
		
		void myplayer::Deinitialize(PluginHost::IShell*service )
		{
		 cout<<"\n deinitializeing the plugin"<<endl;
		}
		// for showing the informatinon about the plugin
		/* virtual */ string myplayer:: Information() const
		{
			return ((_T("The purpose of this plugin is provide ablity to execute functional tests.")));
		}
		
		void *busth(void *arg)
		{
			//cout<<"In side thread\n";
			cout<<"setted state to play\n";
			bus =gst_element_get_bus(pipeline);
			while(pipeline!=NULL)
			{
			
			
			
			
			
			msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, (GstMessageType)(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

			if (msg != NULL) 
			{
				cout<<"inside if\n";
    				GError *err;
    				gchar *debug_info;
    				switch (GST_MESSAGE_TYPE (msg)) {
        			case GST_MESSAGE_ERROR:
        			gst_message_parse_error (msg, &err, &debug_info);
        			g_printerr ("Error received from element %s: %s\n",
        			GST_OBJECT_NAME (msg->src), err->message);
        			g_printerr ("Debugging information: %s\n",
        			debug_info ? debug_info : "none");
        			g_clear_error (&err);
        			g_free (debug_info);
        			break;
      				case GST_MESSAGE_EOS:
        			 g_print ("End-Of-Stream reached.\n");
        			 if(loopvalue==true)
        			 {
        			 	cout<<"in thread and in loop mode loop";
					event = gst_event_new_seek(1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH, GST_SEEK_TYPE_SET, 0, GST_SEEK_TYPE_NONE, 0);  // move the start position
                			gst_element_send_event(pipeline, event);
                			gst_element_set_state(pipeline, GST_STATE_PLAYING);
                		}// Restart the pipeline
            			else
           			{
             				gst_element_set_state (pipeline, GST_STATE_NULL);
             				gst_object_unref (pipeline);
            			}             
        			 break;
      				default:
       					 /* We should n
        				 ot reach here because we only asked for ERRORs and EOS */
        				g_printerr ("Unexpected message received.\n");
        				break;
    					}
    					}
    		gst_message_unref (msg);
 		 }
    	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(pipeline);
	
	pthread_exit(NULL);
	return NULL;

}



		
		uint32_t myplayer::Path(const JsonObject& request)
		{
			
			uint32_t result=Core::ERROR_NONE;
			loopvalue=false;
			if(pipeline!=NULL)
			{
				gst_element_set_state(pipeline,GST_STATE_NULL);
				gst_object_unref(pipeline);
			}
			
			cout<<request["url"].String()<<endl;
			string value ="playbin uri=";
			value.append(request["url"]);
			cout<<value<<endl;
			
			pipeline=gst_parse_launch(value.c_str(),NULL);
			gst_element_set_state(pipeline,GST_STATE_PLAYING);
			pthread_t bus_thread;
			pthread_create (&bus_thread,NULL,busth,NULL);
			
			
			
			cout<<"thread called\n";
			return result;
			cout<<"After return statement";
		}
		uint32_t myplayer::pause()
		{
			GstState state;
			gst_element_get_state(pipeline, &state, NULL, GST_CLOCK_TIME_NONE);
         	if (state == GST_STATE_PLAYING) 
         	{
             		gst_element_set_state(pipeline, GST_STATE_PAUSED);
             		cout<<"video paused";
          	} 
          	else if (state == GST_STATE_PAUSED) 
          	{
             		gst_element_set_state(pipeline, GST_STATE_PLAYING);
             		cout<<"video playing";
         	} 
         	return Core::ERROR_NONE;        
         	}
         	
         	uint32_t myplayer::loop()
         	{
         		if(loopvalue==false)
         		{
         			loopvalue=true;
         			cout<<"Entered loop mode";
         		}
         		else
         		{
         			loopvalue=false;
         			cout<<"exit loop mode";
         		}
         		
         		return Core::ERROR_NONE;
         	}	
			
			
					
	
  }	//namespace plugin
}	//namespace framework
		
		
	
