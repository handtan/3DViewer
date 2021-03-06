﻿using InterProcessCommunicator;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace UI
{
    /// <summary>
    /// ModelControl.xaml 的互動邏輯
    /// </summary>
    public partial class ModelControl : Page
    {
        public IntPtr HostHandle { set; get; }

        private int WM_WPFUI_MODELCONTROL = 1024 + 110;
        public ModelControl()
        {
            InitializeComponent();
        }

        public double GetWidth()
        {
            return ((Grid)this.Content).RenderSize.Width;
        }
        public double GetHeight()
        {
            return ((Grid)this.Content).RenderSize.Height;
        }

        private void Rot_cc_click(object sender, RoutedEventArgs e)
        {
            //MessageBox.Show("rot_cc");
        }

        private void Rot_c_Click(object sender, RoutedEventArgs e)
        {
            //MessageBox.Show("rot_c");
        }

        private void Button_Event_Click(object sender, RoutedEventArgs e)
        {
            var btn = sender as Button;

            switch (btn.Name)
            {
                case "m_rot_cc":
                    SendMessageClass.SendMessage(HostHandle, WM_WPFUI_MODELCONTROL, (IntPtr)0, (IntPtr)1);
                    break;
                case "m_rot_c":
                    SendMessageClass.SendMessage(HostHandle, WM_WPFUI_MODELCONTROL, (IntPtr)0, (IntPtr)0);
                    break;
                case "m_tra_left":
                    SendMessageClass.SendMessage(HostHandle, WM_WPFUI_MODELCONTROL, (IntPtr)1, (IntPtr)4);
                    break;
                case "m_tra_right":
                    SendMessageClass.SendMessage(HostHandle, WM_WPFUI_MODELCONTROL, (IntPtr)1, (IntPtr)5);
                    break;
                case "m_tra_up":
                    SendMessageClass.SendMessage(HostHandle, WM_WPFUI_MODELCONTROL, (IntPtr)1, (IntPtr)2);
                    break;
                case "m_tra_down":
                    SendMessageClass.SendMessage(HostHandle, WM_WPFUI_MODELCONTROL, (IntPtr)1, (IntPtr)3);
                    break;
            }
        }
    }
}
