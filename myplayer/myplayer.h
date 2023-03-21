#pragma once
#include "Module.h"
#include <gst/gst.h>
#include <pthread.h>
GstBus *bus;
GstMessage *msg;
GstElement *pipeline;
GstEvent *event;
static bool loopvalue;


namespace WPEFramework {
	namespace Plugin {
		class myplayer : public PluginHost::IPlugin, public PluginHost::JSONRPC {
		private:
			myplayer(const myplayer&)=delete;
			myplayer& operator=(const myplayer&) =delete;
		public:
			myplayer();
			virtual ~myplayer();
			virtual const string Initialize(PluginHost::IShell* service) override;
			virtual void Deinitialize(PluginHost::IShell* service) override;
			virtual string Information() const override;
			uint32_t Path(const JsonObject& request);
			uint32_t pause();
			uint32_t loop();
			
			BEGIN_INTERFACE_MAP(myplayer)
			INTERFACE_ENTRY(PluginHost::IPlugin)
			INTERFACE_ENTRY(PluginHost::IDispatcher)
			END_INTERFACE_MAP
			};
		}	//namespace Plugin
	}	//namespace WPEFramework
	
	
