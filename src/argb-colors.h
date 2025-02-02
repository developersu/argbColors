int verbose_output = 0;

enum separate_options {
    Z1 = 0x100,
    Z2,  Z3, Z4, Z5, Z6,
    C1, C2, C3, C4, C5, C6, 
    I1, I2, I3, I4, I5, I6,
    B1, B2, B3, B4, B5, B6
};

const char *argp_program_version = "argb-color 0.1\n\n\
License: GPLv3+: GNU GPL version 3 or newer <https://gnu.org/licenses/gpl.html>.\n\
Dmitry Isaenko, 2025, Russia.\n\
https://redrise.ru, https://github.com/developersu";
const char *argp_program_bug_address = "https://github.com/developersu/argbColors/issues/";
static char doc[] = "-s [color|wave|wave2|off]\n-e -z1=[color|off|impulse|flash|flash2|cycle] ... -z6=[...]";
static char doc1[] = "Apply one effect for everything or configure each 'device' separately\v\
Command's related options:\n\
Synchronized\n\
 * color:   color (-c ...)\n\
Separate\n\
 * color:   color (-cN ...), brightness (-bN ...)\n\
 * impulse: color (-cN ...), intensity (-iN ...)\n\
 * flash:   color (-cN ...), brightness (-bN ...), frequency (-iN ...)\n\
 * flash2:  color (-cN ...), brightness (-bN ...), frequency (-iN ...)\n\
 * cycle:   intensity (-iN ...), brightness (-bN ...)\n\n\
Zones description:\n\
 |‾‾|‾‾‾‾‾‾‾‾‾‾‾1| \n\
 | 6| |‾‾| ||||  | \n\
 |  |  ‾‾  ||||  | \n\
 |__|      |‾‾‾| | \n\
 |         |_4_| | \n\
 |___2_3_5_______| \n";

static struct argp_option options[] = {
  {"sync",       's', 0,           0, "Synchronized command flow"   , 0},
  {"separate",   'e', 0,           0, "Separate commands flow"      , 1},
  {"color",      'c', "RGB_color", 0, "Color (000000..ffffff)"      , 2},

  {"brightness", 'b', "value",     0, "Brightness (0..4)"           , 2},

  {"z1", Z1, "command", 0, "Command for zone 1", 3 },
  {"z2", Z2, "command", 0, "Command for zone 2", 3 },
  {"z3", Z3, "command", 0, "Command for zone 3", 3 },
  {"z4", Z4, "command", 0, "Command for zone 4", 3 },
  {"z5", Z5, "command", 0, "Command for zone 5", 3 },
  {"z6", Z6, "command", 0, "Command for zone 6", 3 },
  
  {"c1", C1, "RGB_color", 0, "Color for zone 1", 4 },
  {"c2", C2, "RGB_color", 0, "Color for zone 2", 4 },
  {"c3", C3, "RGB_color", 0, "Color for zone 3", 4 },
  {"c4", C4, "RGB_color", 0, "Color for zone 4", 4 },
  {"c5", C5, "RGB_color", 0, "Color for zone 5", 4 },
  {"c6", C6, "RGB_color", 0, "Color for zone 6", 4 },
  
  {"i1", I1, "value", 0, "Intensity/frequency for zone 1 (0..4)", 4 },
  {"i2", I2, "value", 0, "Intensity/frequency for zone 2 (0..4)", 4 },
  {"i3", I3, "value", 0, "Intensity/frequency for zone 3 (0..4)", 4 },
  {"i4", I4, "value", 0, "Intensity/frequency for zone 4 (0..4)", 4 },
  {"i5", I5, "value", 0, "Intensity/frequency for zone 5 (0..4)", 4 },
  {"i6", I6, "value", 0, "Intensity/frequency for zone 6 (0..4)", 4 },
  
  {"b1", B1, "value", 0, "Brightness for zone 1 (0..4)", 4 },
  {"b2", B2, "value", 0, "Brightness for zone 2 (0..4)", 4 },
  {"b3", B3, "value", 0, "Brightness for zone 3 (0..4)", 4 },
  {"b4", B4, "value", 0, "Brightness for zone 4 (0..4)", 4 },
  {"b5", B5, "value", 0, "Brightness for zone 5 (0..4)", 4 },
  {"b6", B6, "value", 0, "Brightness for zone 6 (0..4)", 4 },

  {"quiet",      'q', 0,           0, "Mute output"                   , 7},
  {"verbose",    'v', 0,           0, "Verbose output"                , 7},
  { 0 }
};

struct arguments{
  char *args[1];
  int quiet;
  int sync;
  int separate;
  char *color,
       *brightness,
       *z1, *z2, *z3, *z4, *z5, *z6,
       *c1, *c2, *c3, *c4, *c5, *c6,
       *i1, *i2, *i3, *i4, *i5, *i6,
       *b1, *b2, *b3, *b4, *b5, *b6;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state){
    /* Get the input argument from argp_parse, which we
       know is a pointer to our arguments structure. */
    struct arguments *arguments = state->input;

    switch (key) {
        case 'q':
            arguments->quiet = 1;
            break;
        case 's':
            arguments->sync = 1;
            break;
        case 'e':
            arguments->separate = 1;
            break;
        case 'c':
            arguments->color = arg;
            break;
        case 'b':
            arguments->brightness = arg;
            break;
        case 'v':
            verbose_output = 1;
        case Z1:
            arguments->z1 = arg;
            break;
        case Z2:
            arguments->z2 = arg;
            break;
	    case Z3:
            arguments->z3 = arg;
            break;
        case Z4:
            arguments->z4 = arg;
            break;
	    case Z5:
            arguments->z5 = arg;
            break;
        case Z6:
            arguments->z6 = arg;
            break;
	    case C1:
            arguments->c1 = arg;
            break;
        case C2:
            arguments->c2 = arg;
            break;
	    case C3:
            arguments->c3 = arg;
            break;
        case C4:
            arguments->c4 = arg;
            break;
	    case C5:
            arguments->c5 = arg;
            break;
        case C6:
            arguments->c6 = arg;
            break;
	    case I1:
            arguments->i1 = arg;
            break;
        case I2:
            arguments->i2 = arg;
            break;
	    case I3:
            arguments->i3 = arg;
            break;
        case I4:
            arguments->i4 = arg;
            break;
    	case I5:
            arguments->i5 = arg;
            break;
        case I6:
            arguments->i6 = arg;
            break;
        case B1:
            arguments->b1 = arg;
            break;
        case B2:
            arguments->b2 = arg;
            break;
	    case B3:
            arguments->b3 = arg;
            break;
        case B4:
            arguments->b4 = arg;
            break;
    	case B5:
            arguments->b5 = arg;
            break;
        case B6:
            arguments->b6 = arg;
            break;
        
        case ARGP_KEY_ARG:
            if (state->arg_num >= 1) // Too many arguments
                argp_usage (state);
            arguments->args[state->arg_num] = arg;
            break;

        case ARGP_KEY_END:
            if (state->arg_num < 1)
                arguments->args[0] = "";
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }
 
    return 0;
}

static struct argp argp = { options, parse_opt, doc, doc1 };