import smtplib

def breach_mail(Type, Classification, Priority, Datetime):
    with smtplib.SMTP('smtp.gmail.com', 587) as smtp:
        smtp.ehlo()
        smtp.starttls()         #TLS for security
        smtp.ehlo()

        smtp.login('licster.breach@gmail.com', 'Lic#v77zX')             # can also be replaced by environment variables in the future of this project to ensure security 
        
        subject = 'A breach has been detected by the Intrusion Detection System!'
        body = 'A '+ Type +' was detected at '+ Datetime +'.'
        msg = f'Subject: {subject}\n\n{body}'
        
        smtp.sendmail('licster.breach@gmail.com', 'licster.breach@gmail.com', msg)







