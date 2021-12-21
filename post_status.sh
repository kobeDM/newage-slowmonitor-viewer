#curl -F file=@status_up.png -F "initial_comment=Status" -F channels=C02CS4T4SH2 -H "Authorization: Bearer xoxp-xxx" https://slack.com/api/files.upload
# First, in channel, install PostTest App
# OAuth & Permissions
# Bot token -> file:write 
# Use Bot User OAuth Token
# channels = darkmatter or C011ZFDE9PW (channel ID)
# Reference: https://api.slack.com/methods/files.upload
# https://api.slack.com/methods/files.upload#examples
#curl -F file=@status_up.png -F "initial_comment=NEWAGE 0.3b status" -F channels=C011ZFDE9PW,C02CS4T4SH2 -H "Authorization: Bearer xoxb-608416396307-2427422884114-k13qeDhtJTU9emBRjd15JBcx" https://slack.com/api/files.upload
#curl -F file=@/home/msgc/status_tmp/status_up.png -F "initial_comment=NEWAGE 0.3b status :smile:" -F channels=C02CS4T4SH2 -H "Authorization: Bearer xoxb-608416396307-2430204519381-R9aNY6Q3jZGib77EBdwDFtnx" https://slack.com/api/files.upload
curl -F file=@/home/msgc/status_tmp/status_up.png -F "initial_comment=NEWAGE 0.3b status :smile:" -F channels=C02CS4T4SH2 -H "Authorization: Bearer xoxb-608416396307-2852133685159-PurjPzlzmEve8jfLEXOI9ddJ" https://slack.com/api/files.upload


