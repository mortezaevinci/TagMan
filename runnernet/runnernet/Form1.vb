Imports System.Windows.Forms
Imports System
Imports System.Diagnostics
Imports System.ComponentModel


Public Class Form1

    Private Sub bOpen_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles bOpen.Click


        If OpenDialog1.ShowDialog() = DialogResult.OK Then
            pTag.Image = Image.FromFile(OpenDialog1.FileName)
            lMessage.Text = ""
            Application.DoEvents()


            FileCopy(OpenDialog1.FileName, "q:\temp\tagman_image1.jpg")

            Dim myProcess As New Process()

            Try                ' Get the path that stores user documents.

                myProcess.StartInfo.UseShellExecute = False
                ' You can start any process, HelloWorld is a do-nothing example.
                myProcess.StartInfo.FileName = "Q:\code\MyMain\TagMan\Release\wcocr.exe"
                myProcess.StartInfo.CreateNoWindow = True
                myProcess.StartInfo.RedirectStandardInput = False
                myProcess.StartInfo.RedirectStandardOutput = True
                myProcess.Start()
                ' This code assumes the process you are starting will terminate itself.  
                ' Given that is is started without a window so you cannot terminate it  
                ' on the desktop, it must terminate itself or you can do it programmatically 
                ' from this application using the Kill method. 
                Dim t1 As String = myProcess.StandardOutput.ReadToEnd()
                Dim s1 = Split(t1, "string is")
                lMessage.Text = s1(0)

            Catch ee As Exception
                lMessage.Text = ee.Message
            End Try

        End If

    End Sub

    Private Sub pTag_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles pTag.Click

    End Sub
End Class
