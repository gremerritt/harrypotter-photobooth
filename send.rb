#require 'twilio-ruby'
require 'dropbox_sdk'
require 'mail'

def main filename
  config_file = File.expand_path("config.yml")
  config = YAML.load_file(config_file)

  url = upload_to_dropbox filename, config

  entry = "zenity --entry --title='Email it!' --text='Enter your email:'"
  email = `#{entry}`

  send_email(email, filename, config) if email_okay?(email)
end

def email_okay? email
  if /.+@.+\..+/i.match email
    return true
  else
    return false
  end
end

def upload_to_dropbox filename, config
  token = config[:dropbox][:token]
  client = DropboxClient.new token

  img = File.read(filename)
  client.put_file("/HPPCP/#{filename}", img, true)
  #media = client.media("/HPPCP/#{filename}")
  #return media['url']
end

# def send_mms to_number, url, config
#   account_sid = config[:twilio][:account_sid]
#   auth_token = config[:twilio][:auth_token]
#
#   client = Twilio::REST::Client.new account_sid, auth_token
#
#   client.messages.create(
#     from: config[:twilio][:from],
#     to: "+1#{to_number}",
#     body: 'HPPCP',
#     media_url: url
#   )
# end

def send_email to_addr, filename, config
  options = { :address              => "smtp.gmail.com",
              :port                 => 587,
              :user_name            => config[:email][:username],
              :password             => config[:email][:password],
              :authentication       => 'plain',
              :enable_starttls_auto => true  }

  Mail.defaults do
    delivery_method :smtp, options
  end

  Mail.deliver do
     to to_addr
     from config[:email][:from]
     subject 'WANTED'
     body ''
     add_file filename
  end
end

main ARGV[0]
